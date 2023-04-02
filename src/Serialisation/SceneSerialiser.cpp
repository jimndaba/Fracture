#include "FracturePCH.h"
#include "SceneSerialiser.h"
#include "Common/UUID.h"
#include "World/Components.h"
#include "World/SceneManager.h"

Fracture::SceneSerialiser::SceneSerialiser(IOMode mode, SerialiseFormat format) :
ISerialiser(mode, format)
{
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::TagComponent* component)
{
	BeginStruct("Tag");
	Property("Name", component->Name);
	Property("IsActive", component->IsActive);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::TransformComponent* component)
{
	BeginStruct("Transform");
	Property("Position", component->Position);
	glm::vec3 angle = glm::eulerAngles(component->Rotation);
	angle = glm::degrees(angle);
	Property("Rotation", angle);
	Property("Scale", component->Scale);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::HierachyComponent* component)
{
	BeginStruct("Hierachy");
	
	Property("HasParent", component->HasParent);
	Property("ParentID", component->Parent);

	BeginCollection("Children");
	for (const auto& child : component->Children)
	{	
		BeginStruct("Child");
		Property("ChildID", child);		
		EndStruct();
	}
	EndCollection();
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::MeshComponent* component)
{
	BeginStruct("Mesh");
	Property("MeshID", component->Mesh);
	Property("MaterialID", component->Material);
	Property("ShaderID", component->Shader);
	Property("MeshType", (int)component->meshType);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::CameraComponent* component)
{
	BeginStruct("Camera");
	Property("Damping", component->Damping);
	Property("EnableDepthOfField", component->EnableDepthOfField);
	Property("Far", component->Far);
	Property("Near", component->Near);
	Property("FocalLength", component->FocalLength);
	Property("FocalRange", component->FocalRange);
	Property("FoV", component->FoV);
	Property("Pitch", component->Pitch);
	Property("Roll", component->Roll);
	Property("Yaw", component->Yaw);
	Property("Position", component->Position);
	Property("Sensitivity", component->Sensitivity);
	Property("Speed", component->Speed);
	Property("Up", component->Up);
	Property("Front", component->Front);
	Property("Right", component->Right);
	Property("IsActiveCamera", component->IsActiveCamera);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::PointlightComponent* component)
{
	BeginStruct("PointlightComponent");
	Property("Compression", component->Compression);
	Property("Diffuse", component->Diffuse);
	Property("Radius", component->Radius);
	Property("Strength", component->Strength);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::SpotlightComponent* component)
{
	BeginStruct("SpotlightComponent");
	Property("Constant", component->Constant);
	Property("Diffuse", component->Diffuse);
	Property("Linear", component->Linear);
	Property("Quadratic", component->Quadratic);
	Property("InnerCutoff", component->InnerCutoff);
	Property("OuterCutoff", component->OutCutoff);
	Property("Strength", component->Strength);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::SunlightComponent* component)
{
	BeginStruct("SunlightComponent");
	Property("Diffuse", component->Diffuse);
	Property("Strength", component->Strength);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ShadowCasterComponent* component)
{
	BeginStruct("ShadowCasterComponent");
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::RigidbodyComponent* component)
{
	BeginStruct("RigidbodyComponent");
	Property("Mass", component->Mass);
	Property("IsDynamic", component->IsDynamic);
	Property("IsKinematic", component->IsKinematic);
	Property("Bouncyness", component->Bouncyness);
	Property("Friction", component->Friction);

	Property("MovementConstraintX", component->LinearConstraints[0]);
	Property("MovementConstraintY", component->LinearConstraints[1]);
	Property("MovementConstraintZ", component->LinearConstraints[2]);

	Property("RotationConstraintX", component->AngularConstraints[0]);
	Property("RotationConstraintY", component->AngularConstraints[1]);
	Property("RotationConstraintZ", component->AngularConstraints[2]);

	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ColliderComponent* component)
{
	BeginStruct("ColliderComponent");
	Property("Type",(int)component->Shape);
	Property("Size",component->Size);
	Property("Radius",component->Radius);
	Property("Height",component->Height);
	Property("Offset",component->Offset);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ScriptComponent* component)
{
	BeginStruct("ScriptComponent");
	Property("Script", component->Script);
	Property("HasScript", component->HasStarted);	
	EndStruct();
}

void Fracture::SceneSerialiser::ReadTagComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Tag"))
	{
		auto tag = std::make_shared<TagComponent>(entity_id, STRING("Name"));
		tag->IsActive = BOOL("IsActive");
		SceneManager::AddComponentByInstance<TagComponent>(entity_id, tag);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadTransformComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Transform"))
	{
		auto transform = std::make_shared<TransformComponent>(entity_id);
		transform->Position = VEC3("Position");
		transform->Scale = VEC3("Scale");
		transform->Rotation = glm::quat(glm::radians(VEC3("Rotation")));
		SceneManager::AddComponentByInstance<TransformComponent>(entity_id, transform);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadHierachyComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Hierachy"))
	{
		auto comp = std::make_shared<HierachyComponent>(entity_id);
		comp->Parent = ID("ParentID");
		comp->HasParent = BOOL("HasParent");
		BeginCollection("Children");
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Child"))
			{
				comp->Children.push_back(ID("ChildID"));
				EndStruct();
			}			
			NextInCollection();
		}
		EndCollection();
		SceneManager::AddComponentByInstance<HierachyComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadMeshComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Mesh"))
	{
		auto comp = std::make_shared<MeshComponent>(entity_id);
		comp->Mesh = ID("MeshID");
		comp->Shader = ID("ShaderID");
		comp->Material = ID("MaterialID");
		comp->meshType = (MeshComponent::MeshType)INT("MeshType");
				
		MeshesToLoad[comp->Mesh] += 1;
		SceneManager::AddComponentByInstance<MeshComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadSpotlightComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("SpotlightComponent"))
	{
		auto comp = std::make_shared<SpotlightComponent>(entity_id);
		comp->Diffuse = VEC3("Diffuse");
		comp->Strength = FLOAT("Strength");
		comp->Constant = FLOAT("Constant");
		comp->Linear = FLOAT("Linear");
		comp->Quadratic = FLOAT("Quadratic");
		comp->InnerCutoff = FLOAT("InnerCutoff");
		comp->OutCutoff = FLOAT("OuterCutoff");
		SceneManager::AddComponentByInstance<SpotlightComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadPointlightComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("PointlightComponent"))
	{
		auto comp = std::make_shared<PointlightComponent>(entity_id);
		comp->Diffuse = VEC3("Diffuse");
		comp->Strength = FLOAT("Strength");
		comp->Compression = FLOAT("Compression");
		comp->Radius = FLOAT("Radius");		
		SceneManager::AddComponentByInstance<PointlightComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadSunlightComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("SunlightComponent"))
	{
		auto comp = std::make_shared<SunlightComponent>(entity_id);
		comp->Diffuse = VEC3("Diffuse");
		comp->Strength = FLOAT("Strength");				
		SceneManager::AddComponentByInstance<SunlightComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadRigidbodyComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("RigidbodyComponent"))
	{
		auto comp = std::make_shared<RigidbodyComponent>(entity_id);
		comp->Mass = FLOAT("Mass");
		comp->Friction = FLOAT("Friction");
		comp->Bouncyness = FLOAT("Bouncyness");
		comp->IsDynamic = BOOL("IsDynamic");
		comp->IsKinematic = BOOL("IsKinematic");

		comp->LinearConstraints[0] = FLOAT("MovementConstraintX");
		comp->LinearConstraints[1] = FLOAT("MovementConstraintY");
		comp->LinearConstraints[2] = FLOAT("MovementConstraintZ");
		
		comp->AngularConstraints[0] = FLOAT("RotationConstraintX");
		comp->AngularConstraints[1] = FLOAT("RotationConstraintY");
		comp->AngularConstraints[2] = FLOAT("RotationConstraintZ");

		SceneManager::AddComponentByInstance<RigidbodyComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadColliderComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("ColliderComponent"))
	{
		auto comp = std::make_shared<ColliderComponent>(entity_id);
		comp->Size = VEC3("Size");
		comp->Shape = (ColliderType)INT("Type");
		comp->Radius = FLOAT("Radius");
		comp->Height = FLOAT("Height");
		comp->Offset = VEC3("Offset");
		SceneManager::AddComponentByInstance<ColliderComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadScriptComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("ScriptComponent"))
	{
		auto comp = std::make_shared<ScriptComponent>(entity_id);
		comp->Script = ID("Script");		
		comp->HasStarted = BOOL("HasScript");		
		SceneManager::AddComponentByInstance<ScriptComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadCameraComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Camera"))
	{
		auto camera = std::make_shared<CameraComponent>(entity_id);
		camera->IsActiveCamera = BOOL("IsActiveCamera");

		camera->Position = VEC3("Position");
		camera->TargetPosition = camera->Position;

		camera->Pitch = FLOAT("Pitch");
		camera->TargetPitch = camera->TargetPitch;

		camera->Yaw = FLOAT("Yaw");
		camera->TargetYaw = camera->Yaw;

		camera->Roll = FLOAT("Roll");
		camera->TargetRoll = camera->Roll;
		

		camera->FoV = FLOAT("FOV");
		camera->TargetFoV = camera->FoV;

		camera->Up = VEC3("Up");
		camera->Right = VEC3("Right");
		camera->Front = VEC3("Front");

				
		camera->EnableDepthOfField = BOOL("EnableDepthOfField");
		camera->Damping = FLOAT("Damping");
		camera->Far = FLOAT("Far");
		camera->Near = FLOAT("Neaer");
		camera->FocalLength = FLOAT("FocalLength");
		camera->FocalRange = FLOAT("FocalRange");
	
		camera->Speed = FLOAT("Speed");
		camera->Sensitivity = FLOAT("Sensitivity");
	
		SceneManager::AddComponentByInstance<CameraComponent>(entity_id, camera);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::WriteScene(Fracture::Scene* scene)
{
	BeginStruct("Scene");
	{
		Property("ID", scene->ID);
		Property("RootID", scene->RootID);
		Property("ActiveCamera", scene->ActiveCameraID);

		BeginCollection("Entities");
		for (const auto& entity : scene->Entities)
		{
			BeginStruct("Entity");
			{
				Property("Entity_ID", entity->ID);

				BeginCollection("Components");
				{
					WriteEntityComponentOfType<TagComponent>(entity->ID);
					WriteEntityComponentOfType<TransformComponent>(entity->ID);
					WriteEntityComponentOfType<HierachyComponent>(entity->ID);
					WriteEntityComponentOfType<MeshComponent>(entity->ID);
					WriteEntityComponentOfType<RigidbodyComponent>(entity->ID);
					WriteEntityComponentOfType<PointlightComponent>(entity->ID);
					WriteEntityComponentOfType<SpotlightComponent>(entity->ID);
					WriteEntityComponentOfType<SunlightComponent>(entity->ID);
					WriteEntityComponentOfType<ShadowCasterComponent>(entity->ID);
					WriteEntityComponentOfType<ColliderComponent>(entity->ID);
					WriteEntityComponentOfType<ScriptComponent>(entity->ID);
				}
				EndCollection();

				BeginCollection("Scripts");
				{
					for (const auto& script : SceneManager::mScript_Entities)
					{
						auto it = std::find_if(std::begin(script.second), std::end(script.second), [&](Fracture::UUID p) { return p == entity->ID; });

						if (it != script.second.end())
						{
							BeginStruct("LuaScript");
							Property("ScriptID", script.first);
							EndStruct();
						}
					}
				}
				EndCollection();
			}
			EndStruct();
		}
		EndCollection();
	}
	EndStruct();
}

std::shared_ptr<Fracture::Scene>  Fracture::SceneSerialiser::ReadScene()
{
	if (BeginStruct("Scene"))
	{
		auto new_Scene = SceneManager::CreateNewScene(ID("RootID"));
		new_Scene->ID = ID("ID");
		new_Scene->RootID = ID("RootID");
		new_Scene->ActiveCameraID = ID("ActiveCamera");	
	
		SceneManager::SetScene(new_Scene);

		if (BeginCollection("Entities"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Entity"))
				{
					auto entity_id = ID("Entity_ID");
					SceneManager::AddEntity(entity_id);

					if (BeginCollection("Components"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							ReadTagComponentIfExists(entity_id);
							ReadTransformComponentIfExists(entity_id);
							ReadHierachyComponentIfExists(entity_id);
							ReadMeshComponentIfExists(entity_id);
							ReadSpotlightComponentIfExists(entity_id);
							ReadPointlightComponentIfExists(entity_id);
							ReadSunlightComponentIfExists(entity_id);
							ReadRigidbodyComponentIfExists(entity_id);
							ReadColliderComponentIfExists(entity_id);
							ReadScriptComponentIfExists(entity_id);
							NextInCollection();
						}
						EndCollection();
					}

					if (BeginCollection("Scripts"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							if (BeginStruct("LuaScript"))
							{
								auto script_id = ID("ScriptID");
								SceneManager::AttachScript(entity_id, script_id);
								EndStruct();
							}
							NextInCollection();
						}
						EndCollection();
					}
					EndStruct();
				}
				NextInCollection();
			}

			EndCollection();
		}
		EndStruct();

		return new_Scene;
	}
}