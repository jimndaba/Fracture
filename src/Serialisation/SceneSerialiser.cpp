#include "FracturePCH.h"
#include "SceneSerialiser.h"
#include "Common/UUID.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "World/WorldEvents.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"

Fracture::SceneSerialiser::SceneSerialiser(IOMode mode, SerialiseFormat format) :
ISerialiser(mode, format)
{
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::TagComponent* component)
{
	BeginStruct("Tag");
	Property("Name", component->Name);
	Property("IsActive", component->IsActive);
	Property("Tags", component->Tags);
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
	Property("Materials", component->Materials);
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

	Property("TargetFoV", component->TargetFoV);
	Property("TargetPitch", component->TargetPitch);
	Property("TargetRoll", component->TargetRoll);
	Property("TargetYaw", component->TargetYaw);
	Property("TargetPosition", component->TargetPosition);

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
	Property("IsTrigger",component->IsTrigger);
	Property("CollisionLayer",(int)component->CollisionLayer);
	Property("CollisionGroup",(int)component->CollisionGroup);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ScriptComponent* component)
{
	BeginStruct("ScriptComponent");
	Property("Script", component->Script);
	Property("HasScript", component->HasStarted);	
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::AudioSourceComponent* component)
{
	BeginStruct("AudioSourceComponent");
	Property("AudioClip", component->AudioClip);
	Property("Volume", component->Volume);
	Property("Pan", component->Pan);
	Property("Is3DSource", component->Is3DSource);
	Property("Looping", component->Looping);
	Property("Mute", component->Mute);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::SkyboxComponent* component)
{
	BeginStruct("SkyboxComponent");
	Property("SkyTexture", component->SkyTexture);	
	Property("IsSkyTextureSet", component->IsSkyTextureSet);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::LightProbeComponent* component)
{
	BeginStruct("LightProbeComponent");
	Property("PREFILTER Resolution", component->BRDFResolution);
	Property("BRDFLUT Resolution", component->BRDFLUTResolution);
	Property("IRRADIANCE Resolution", component->LightProbeResolution);
	Property("IsInterior", component->IsInterior);
	Property("AutoBake", component->AutoBaked);
	Property("ProbePositions", component->ProbePositions);
	Property("Type",(int)component->ProbeType);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::AnimationComponent* component)
{
	BeginStruct("AnimationComponent");
	Property("Mesh", component->Mesh);
	Property("Graph", component->CurrentGraph);
	Property("HasGraph", component->IsGraphSet);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::CharacterControllerComponent* component)
{
	BeginStruct("CharacterControllerComponent");
	Property("CollisionGroup", component->CollisionGroup);
	Property("CollisionLayer", component->CollisionLayer);
	Property("Height", component->Height);
	Property("Radius", component->Radius);
	Property("Size", component->Size);
	Property("Shape", (int)component->Shape);
	Property("ContactOffset", component->Offset);
	Property("MaxSlope", component->MaxSlopeAngle);
	Property("MaxSpeed", component->MaxSpeed);
	Property("StepHeight", component->StepHeight);
	Property("MinDist", component->MinMovementDist);
	EndStruct();

}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ParticleSystemComponent* component)
{
	BeginStruct("ParticleSystemComponent");
	Property("IsFxAttached", component->IsFXAttached);
	Property("FxID", component->particleFXID);	
	EndStruct();
}

void Fracture::SceneSerialiser::ReadTagComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Tag"))
	{
		auto tag = std::make_shared<TagComponent>(entity_id, STRING("Name"));
		tag->IsActive = BOOL("IsActive");
		tag->Tags = STRINGS_VECTOR("Tags");
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

void Fracture::SceneSerialiser::ReadTransformComponentIfExists(Fracture::UUID entity_id, glm::vec3 Position, glm::vec3 scale, glm::quat Rotation)
{
	if (BeginStruct("Transform"))
	{
		auto transform = std::make_shared<TransformComponent>(entity_id);
		transform->Position = Position;
		transform->Scale = scale;
		transform->Rotation = Rotation;
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

void Fracture::SceneSerialiser::ReadHierachyComponentIfExists(Fracture::UUID entity_id, Fracture::UUID new_parent)
{
	if (BeginStruct("Hierachy"))
	{
		auto comp = std::make_shared<HierachyComponent>(entity_id);			
		{
			const auto& parent = SceneManager::GetComponent<HierachyComponent>(new_parent);		
			parent->Prefabs.push_back(entity_id);
			comp->HasParent = true;
			comp->Parent = new_parent;
		}
		SceneManager::AddComponentByInstance<HierachyComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadMeshComponentIfExists(Fracture::UUID entity_id, bool isPrefab, Fracture::UUID prefab_id)
{
	if (BeginStruct("Mesh"))
	{
		auto comp = std::make_shared<MeshComponent>(entity_id);
		comp->Mesh = ID("MeshID");
		comp->Materials = UINT32_VECTOR("Materials");
		comp->meshType = (MeshComponent::MeshType)INT("MeshType");
		comp->IsPrefab = isPrefab;
		comp->PrefabID = prefab_id;
		MeshesToLoad[comp->Mesh] += 1;
		SceneManager::AddComponentByInstance<MeshComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::InstanceMeshComponentIfExists(Fracture::UUID entity_id, Fracture::UUID prefab_id, Fracture::UUID scene_id)
{
	if (BeginStruct("Mesh"))
	{
		auto comp = std::make_shared<PrefabInstanceComponent>(entity_id,prefab_id);
		comp->Mesh = ID("MeshID");
		comp->Materials = UINT32_VECTOR("Materials");
		comp->meshType = (PrefabInstanceComponent::MeshType)INT("MeshType");
		MeshesToLoad[comp->Mesh] += 1;
		SceneManager::AddComponentByInstance<PrefabInstanceComponent>(entity_id, comp);
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
		comp->IsTrigger = BOOL("IsTrigger");
		comp->CollisionLayer = ID("CollisionLayer");
		comp->CollisionGroup = ID("CollisionGroup");
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

		if (BeginCollection("Properties"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Property"))
				{
					auto prop = std::make_shared<ScriptProperty>();

					prop->Type = (PROPERTY_TYPE)INT("Type");
					prop->Name = STRING("Name");

					switch (prop->Type)
					{
					case PROPERTY_TYPE::BOOL:
					{
						prop->Bool = BOOL("Value");
						break;
					}
					case PROPERTY_TYPE::UUID:
					{
						prop->ID = ID("Value");
						break;
					}
					case PROPERTY_TYPE::FLOAT:
					{
						prop->Float = FLOAT("Value");
						break;
					}
					case PROPERTY_TYPE::INT:
					{
						prop->Int = INT("Value");
						break;
					}
					case PROPERTY_TYPE::VEC2:
					{
						prop->Vec2 = VEC2("Value");
						break;
					}
					case PROPERTY_TYPE::VEC3:
					{
						prop->Vec3 = VEC3("Value");
						break;
					}
					case PROPERTY_TYPE::VEC4:
					{
						prop->Vec4 = VEC4("Value");
						break;
					}
					}

					comp->mProperties[STRING("Name")] = prop;

					EndStruct();
				}
				NextInCollection();
			}
			EndCollection();
		}
		
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
		camera->TargetPosition = VEC3("TargetPosition");

		camera->Pitch = FLOAT("Pitch");
		camera->TargetPitch = FLOAT("TargetPitch");

		camera->Yaw = FLOAT("Yaw");
		camera->TargetYaw = FLOAT("TargetYaw");

		camera->Roll = FLOAT("Roll");
		camera->TargetRoll = FLOAT("TargetRoll");
		

		camera->FoV = FLOAT("FoV");
		camera->TargetFoV = FLOAT("TargetFoV");

		camera->Up = VEC3("Up");
		camera->Right = VEC3("Right");
		camera->Front = VEC3("Front");

				
		camera->EnableDepthOfField = BOOL("EnableDepthOfField");
		camera->Damping = FLOAT("Damping");
		camera->Far = FLOAT("Far");
		camera->Near = FLOAT("Near");
		camera->FocalLength = FLOAT("FocalLength");
		camera->FocalRange = FLOAT("FocalRange");
	
		camera->Speed = FLOAT("Speed");
		camera->Sensitivity = FLOAT("Sensitivity");
	
		SceneManager::AddComponentByInstance<CameraComponent>(entity_id, camera);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadAudioSourceComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("AudioSourceComponent"))
	{
		auto comp = std::make_shared<AudioSourceComponent>(entity_id);
		comp->AudioClip = ID("AudioClip");
		comp->Pan = FLOAT("Pan");
		comp->Volume = FLOAT("Pan");
		comp->Is3DSource = BOOL("Is3DSource");
		comp->Looping = BOOL("Looping");
		comp->Mute = BOOL("Mute");
		SceneManager::AddComponentByInstance<AudioSourceComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadSkyboxComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("SkyboxComponent"))
	{
		auto comp = std::make_shared<SkyboxComponent>(entity_id);
		comp->SkyTexture = ID("SkyTexture");
		comp->IsSkyTextureSet = BOOL("IsSkyTextureSet");
		comp->IsDirty = true;
		SceneManager::AddComponentByInstance<SkyboxComponent>(entity_id, comp);

		const auto& txtr = Fracture::AssetManager::GetTextureByID(comp->SkyTexture);
		if (txtr)
			FRACTURE_INFO("SKy texture loaded");	

		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadLightProbeComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("LightProbeComponent"))
	{
		auto comp = std::make_shared<LightProbeComponent>(entity_id);
		comp->IsInterior = BOOL("IsInterior");
		comp->AutoBaked = BOOL("AutoBake");
		comp->ProbeType = (LightProbeComponent::LightProbeType)INT("Type");
		comp->ProbePositions = VEC4_VECTOR("ProbePositions");
		comp->BRDFLUTResolution = INT("BRDFLUT Resolution");
		comp->BRDFResolution = INT("PREFILTER Resolution");
		comp->LightProbeResolution = INT("IRRADIANCE Resolution");		
		SceneManager::AddComponentByInstance<LightProbeComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadAnimationComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("AnimationComponent"))
	{
		auto comp = std::make_shared<AnimationComponent>(entity_id);
		comp->Mesh = ID("Mesh");
		comp->CurrentGraph = ID("Graph");		
		comp->IsGraphSet = BOOL("HasGraph");		

		AssetManager::Instance()->GetAnimationGraphByID(comp->CurrentGraph);

		SceneManager::AddComponentByInstance<AnimationComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadCharacterControllerComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("CharacterControllerComponent"))
	{
		auto comp = std::make_shared<CharacterControllerComponent>(entity_id);
		comp->CollisionGroup = INT("CollisionGroup");
		comp->CollisionLayer = INT("CollisionLayer");
		comp->Height = FLOAT("Height");
		comp->Radius = FLOAT("Radius");
		comp->StepHeight = FLOAT("StepHeight");
		comp->MaxSlopeAngle = FLOAT("MaxSlope");
		comp->MaxSpeed = FLOAT("MaxSpeed");
		comp->MinMovementDist = FLOAT("MinDist");
		comp->Size = VEC3("Size");
		comp->Offset = VEC3("ContactOffset");
		comp->Shape = (Fracture::CCColliderType)INT("Shape");
		SceneManager::AddComponentByInstance<CharacterControllerComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadParticleSystemComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("ParticleSystemComponent"))
	{
		auto comp = std::make_shared<ParticleSystemComponent>(entity_id);
		comp->IsFXAttached = BOOL("IsFxAttached");
		comp->particleFXID = ID("FxID");
		SceneManager::AddComponentByInstance<ParticleSystemComponent>(entity_id, comp);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::WriteScene(Fracture::Scene* scene)
{
	BeginStruct("Scene");
	{
		Property("ID", scene->ID);
		Property("ActiveCamera", scene->ActiveCameraID);
		Property("RootID", scene->RootID);
		Property("Name", scene->Name);

		BeginStruct("Root");
		{			
			WriteEntityComponentOfType<TagComponent>(scene->RootID);
			WriteEntityComponentOfType<TransformComponent>(scene->RootID);
			WriteEntityComponentOfType<HierachyComponent>(scene->RootID);
			WriteEntityComponentOfType<MeshComponent>(scene->RootID);
			WriteEntityComponentOfType<RigidbodyComponent>(scene->RootID);
			WriteEntityComponentOfType<CameraComponent>(scene->RootID);
			WriteEntityComponentOfType<PointlightComponent>(scene->RootID);
			WriteEntityComponentOfType<SpotlightComponent>(scene->RootID);
			WriteEntityComponentOfType<SunlightComponent>(scene->RootID);
			WriteEntityComponentOfType<ShadowCasterComponent>(scene->RootID);
			WriteEntityComponentOfType<ColliderComponent>(scene->RootID);			
			WriteEntityComponentOfType<SkyboxComponent>(scene->RootID);
			WriteEntityComponentOfType<LightProbeComponent>(scene->RootID);
			WriteEntityComponentOfType<AnimationComponent>(scene->RootID);
			WriteEntityComponentOfType<CharacterControllerComponent>(scene->RootID);
			WriteEntityComponentOfType<ParticleSystemComponent>(scene->RootID);

			BeginCollection("Scripts");
			{
				for (const auto& script : SceneManager::mScript_Entities)
				{
					auto it = std::find_if(std::begin(script.second), std::end(script.second), [&](Fracture::UUID p) { return p == scene->RootID; });

					if (it != script.second.end())
					{
						BeginStruct("LuaScript");
						Property("ScriptID", script.first);

						BeginCollection("Properties");
						{
							for (const auto& prop : ScriptManager::mScripts[script.first]->GetProperties())
							{
								BeginStruct("Property");
								switch (prop.second->Type)
								{
								case PROPERTY_TYPE::UUID:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", (uint32_t)prop.second->ID);
									break;
								}
								case PROPERTY_TYPE::BOOL:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Bool);
									break;
								}
								case PROPERTY_TYPE::COLOR3:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Color3);
									break;
								}
								case PROPERTY_TYPE::COLOR4:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Color4);
									break;
								}
								case PROPERTY_TYPE::FLOAT:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Float);
									break;
								}
								case PROPERTY_TYPE::INT:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Int);
									break;
								}
								case PROPERTY_TYPE::STRING:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->String);
									break;
								}
								case PROPERTY_TYPE::VEC2:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Vec2);
									break;
								}
								case PROPERTY_TYPE::VEC3:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Vec3);
									break;
								}
								case PROPERTY_TYPE::VEC4:
								{
									Property("Name", prop.second->Name);
									Property("Type", (int)prop.second->Type);
									Property("Value", prop.second->Vec4);
									break;
								}
								}
								EndStruct();
							}
						}
						EndCollection();
						EndStruct();
					}
				}
			}
			EndCollection();

			EndStruct();

		}

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
					WriteEntityComponentOfType<CameraComponent>(entity->ID);
					WriteEntityComponentOfType<RigidbodyComponent>(entity->ID);
					WriteEntityComponentOfType<PointlightComponent>(entity->ID);
					WriteEntityComponentOfType<SpotlightComponent>(entity->ID);
					WriteEntityComponentOfType<SunlightComponent>(entity->ID);
					WriteEntityComponentOfType<ShadowCasterComponent>(entity->ID);
					WriteEntityComponentOfType<ColliderComponent>(entity->ID);					
					WriteEntityComponentOfType<SkyboxComponent>(entity->ID);
					WriteEntityComponentOfType<LightProbeComponent>(entity->ID);
					WriteEntityComponentOfType<AnimationComponent>(entity->ID);
					WriteEntityComponentOfType<CharacterControllerComponent>(entity->ID);
					WriteEntityComponentOfType<ParticleSystemComponent>(entity->ID);
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
							BeginCollection("Properties");
							{
								for (const auto& prop : ScriptManager::mScripts[script.first]->GetProperties())
								{
									BeginStruct("Property");
									switch (prop.second->Type)
									{
									case PROPERTY_TYPE::UUID:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", (uint32_t)prop.second->ID);
										break;
									}
									case PROPERTY_TYPE::BOOL:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Bool);
										break;
									}
									case PROPERTY_TYPE::COLOR3:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Color3);
										break;
									}
									case PROPERTY_TYPE::COLOR4:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Color4);
										break;
									}
									case PROPERTY_TYPE::FLOAT:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Float);
										break;
									}
									case PROPERTY_TYPE::INT:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Int);
										break;
									}
									case PROPERTY_TYPE::STRING:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->String);
										break;
									}
									case PROPERTY_TYPE::VEC2:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Vec2);
										break;
									}
									case PROPERTY_TYPE::VEC3:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Vec3);
										break;
									}
									case PROPERTY_TYPE::VEC4:
									{
										Property("Name", prop.second->Name);
										Property("Type", (int)prop.second->Type);
										Property("Value", prop.second->Vec4);
										break;
									}
									}
									EndStruct();
								}
							}
							EndCollection();
							EndStruct();
						}
					}
				}
				EndCollection();
			}
			EndStruct();
		}
		EndCollection();


		BeginCollection("Prefabs");
		for (const auto& prefab : scene->mPrefabs)
		{
			BeginStruct("Prefab");
			{
				Property("Prefab_ID", prefab.PrefabID);				
				Property("Scene_ID", prefab.SceneID);				
				Property("Parent_ID", prefab.ParentID);	
				const auto& transform = SceneManager::GetComponent<TransformComponent>(prefab.PrefabID);
				if (transform)
				{
					Property("Position", transform->Position);
					glm::vec3 angle = glm::eulerAngles(transform->Rotation);
					angle = glm::degrees(angle);
					Property("Rotation", angle);
					Property("Scale", transform->Scale);
				}
				else
				{
					Property("Position", prefab.Position);
					glm::vec3 angle = glm::eulerAngles(prefab.Rotation);
					angle = glm::degrees(angle);
					Property("Rotation", angle);
					Property("Scale", prefab.Scale);
				}
			}
			EndStruct();
		}
		EndCollection();
	}
	EndStruct();
}

void Fracture::SceneSerialiser::EntitiesToPrefab(PrefabCreationInfo info)
{
	BeginStruct("Scene");
	{
		Property("ID", info.SceneID);
		Property("ActiveCamera", 0);
		Property("RootID", UUID());
		Property("Name", info.Name);

		auto root_id = UUID();
		BeginStruct("Root");
		{
			{
			auto tag = TagComponent(root_id, info.Name);
			SerialiseComponent(&tag);
			}		
			{
				auto new_component = TransformComponent(root_id);
				SerialiseComponent(&new_component);
			}
			{
				auto new_component = HierachyComponent(root_id);
				SerialiseComponent(&new_component);
			}		
			EndStruct();
		}

		BeginCollection("Entities");
		for (int i = 0; i < info.Entities.size();i++)
		{
			WriteEntityToPrefab(root_id, info.Entities[i]);
		}
		EndCollection();

		BeginCollection("Prefabs");
		for (int i = 0 ; i < info.Prefabs.size() ;i++)
		{
			BeginStruct("Prefab");
			{
		
			}
			EndStruct();
		}
		EndCollection();
	}
	EndStruct();
}

void Fracture::SceneSerialiser::WriteEntityToPrefab(Fracture::UUID& parent, Fracture::UUID& entity)
{
	if (SceneManager::HasComponent<HierachyComponent>(entity))
	{
		auto& hierachy = *SceneManager::GetComponent<HierachyComponent>(entity);
		auto entityid = UUID();

		BeginStruct("Entity");
		{
			Property("Entity_ID", entityid);
			BeginCollection("Components");
			{
				if (SceneManager::HasComponent<TagComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<TagComponent>(entity);
					auto new_component = TagComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<TransformComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<TransformComponent>(entity);
					auto new_component = TransformComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				{
					auto new_component = HierachyComponent(hierachy, entityid);
					new_component.HasParent = true;
					new_component.Parent = parent;
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<MeshComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<MeshComponent>(entity);
					auto new_component = MeshComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<RigidbodyComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<RigidbodyComponent>(entity);
					auto new_component = RigidbodyComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<ColliderComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<ColliderComponent>(entity);
					auto new_component = ColliderComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<CameraComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<CameraComponent>(entity);
					auto new_component = CameraComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<SpotlightComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<SpotlightComponent>(entity);
					auto new_component = SpotlightComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<SunlightComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<SunlightComponent>(entity);
					auto new_component = SunlightComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<ShadowCasterComponent>(entity))
				{
				}
				if (SceneManager::HasComponent<ColliderComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<ColliderComponent>(entity);
					auto new_component = ColliderComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<ScriptComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<ScriptComponent>(entity);
					auto new_component = ScriptComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<SkyboxComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<SkyboxComponent>(entity);
					auto new_component = SkyboxComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<LightProbeComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<LightProbeComponent>(entity);
					auto new_component = LightProbeComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<AnimationComponent>(entity))
				{
					if (SceneManager::GetComponent<AnimationComponent>(entity))
					{
						auto& component = *SceneManager::GetComponent<AnimationComponent>(entity);
						auto new_component = AnimationComponent(component, entityid);
						SerialiseComponent(&new_component);
					}
				}
				if (SceneManager::HasComponent<CharacterControllerComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<CharacterControllerComponent>(entity);
					auto new_component = CharacterControllerComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				if (SceneManager::HasComponent<ParticleSystemComponent>(entity))
				{
					auto& component = *SceneManager::GetComponent<ParticleSystemComponent>(entity);
					auto new_component = ParticleSystemComponent(component, entityid);
					SerialiseComponent(&new_component);
				}
				EndCollection();
			}
		}
		EndStruct();

		for (auto& child : hierachy.Children)
		{
			WriteEntityToPrefab(entityid, child);
		}
	}
}

std::shared_ptr<Fracture::Scene>  Fracture::SceneSerialiser::ReadScene()
{
	if (BeginStruct("Scene"))
	{
		auto new_Scene = SceneManager::CreateNewScene(ID("RootID"));
		new_Scene->ID = ID("ID");
		new_Scene->RootID = ID("RootID");
		new_Scene->ActiveCameraID = ID("ActiveCamera");
		new_Scene->Name = STRING("Name");

		SceneManager::SetScene(new_Scene);
		if (BeginStruct("Root"))
		{			
			ReadTagComponentIfExists(new_Scene->RootID);
			ReadTransformComponentIfExists(new_Scene->RootID);
			ReadHierachyComponentIfExists(new_Scene->RootID);
			ReadMeshComponentIfExists(new_Scene->RootID);
			ReadSpotlightComponentIfExists(new_Scene->RootID);
			ReadPointlightComponentIfExists(new_Scene->RootID);
			ReadSunlightComponentIfExists(new_Scene->RootID);
			ReadRigidbodyComponentIfExists(new_Scene->RootID);
			ReadColliderComponentIfExists(new_Scene->RootID);
			ReadCameraComponentIfExists(new_Scene->RootID);
			ReadScriptComponentIfExists(new_Scene->RootID);
			ReadSkyboxComponentIfExists(new_Scene->RootID);
			ReadLightProbeComponentIfExists(new_Scene->RootID);
			ReadAnimationComponentIfExists(new_Scene->RootID);
			ReadCharacterControllerComponentIfExists(new_Scene->RootID);
			ReadParticleSystemComponentIfExists(new_Scene->RootID);

			if (BeginCollection("Scripts"))
			{
				while (CurrentCollectionIndex() < GetCollectionSize())
				{
					if (BeginStruct("LuaScript"))
					{
						auto script_id = ID("ScriptID");
						SceneManager::AttachScript(new_Scene->RootID, script_id);
						ScriptManager::LoadScriptProperties(script_id);
						/*
						const auto& luascript = ScriptManager::GetLuaScript(script_id);
						if (BeginCollection("Properties"))
						{
							//auto properties = luascript->GetProperties();
							while (CurrentCollectionIndex() < GetCollectionSize())
							{
								if (BeginStruct("Property"))
								{
									switch ((PROPERTY_TYPE)INT("Type"))
									{
									case PROPERTY_TYPE::UUID:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::UUID;
										prop->ID = ID("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::BOOL:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::BOOL;
										prop->Bool = BOOL("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::COLOR3:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::COLOR3;
										prop->Color3 = VEC3("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::COLOR4:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::COLOR4;
										prop->Color4 = VEC4("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::FLOAT:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::FLOAT;
										prop->Float = FLOAT("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::INT:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::INT;
										prop->Int = INT("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::STRING:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::STRING;
										prop->String = STRING("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									}
									EndStruct();
								}
								NextInCollection();
							}
							EndCollection();
						}
						*/
						EndStruct();
					}
					NextInCollection();
				}
				EndCollection();
			}

			EndStruct();
		}

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
							ReadCameraComponentIfExists(entity_id);
							ReadSunlightComponentIfExists(entity_id);
							ReadRigidbodyComponentIfExists(entity_id);
							ReadColliderComponentIfExists(entity_id);
							//ReadScriptComponentIfExists(entity_id);
							ReadSkyboxComponentIfExists(entity_id);
							ReadLightProbeComponentIfExists(entity_id);
							ReadAnimationComponentIfExists(entity_id);
							ReadCharacterControllerComponentIfExists(entity_id);
							ReadParticleSystemComponentIfExists(entity_id);
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
								const auto& luascript = ScriptManager::GetLuaScript(script_id);
								SceneManager::AttachScript(entity_id, script_id);
								ScriptManager::LoadScriptProperties(script_id);
								/*
								const auto& luascript = ScriptManager::GetLuaScript(script_id);
								if (BeginCollection("Properties"))
								{
									//auto properties = luascript->GetProperties();
									while (CurrentCollectionIndex() < GetCollectionSize())
									{
										if (BeginStruct("Property"))
										{
											switch ((PROPERTY_TYPE)INT("Type"))
											{
											case PROPERTY_TYPE::UUID:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::UUID;
												prop->ID = ID("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::BOOL:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::BOOL;
												prop->Bool = BOOL("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR3:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR3;
												prop->Color3 = VEC3("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR4:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR4;
												prop->Color4 = VEC4("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::FLOAT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::FLOAT;
												prop->Float = FLOAT("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::INT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::INT;
												prop->Int = INT("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::STRING:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::STRING;
												prop->String = STRING("Value");
												luascript->m_Properties[prop->Name] = prop;
												break;
											}
											}
											EndStruct();
										}
										NextInCollection();
									}
									EndCollection();
								}
								*/
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

		if (BeginCollection("Prefabs"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Prefab"))
				{
					ScenePrefab prefab;
					prefab.PrefabID = ID("Prefab_ID");
					prefab.SceneID = ID("Scene_ID");
					prefab.ParentID = ID("Parent_ID");					
					prefab.Position = VEC3("Position");
					prefab.Scale = VEC3("Scale");
					prefab.Rotation = glm::quat(glm::radians(VEC3("Rotation")));
				    Eventbus::Publish<Fracture::InstanceScenePrefabEvent>(prefab);
					EndStruct();
				}
				NextInCollection();
			}
			EndCollection();
		}

		EndStruct();

		return new_Scene;
	}

	return nullptr;
}

std::shared_ptr<Fracture::Scene> Fracture::SceneSerialiser::ReadSceneWithoutLoad()
{
	if (BeginStruct("Scene"))
	{
		auto new_Scene = SceneManager::CreateNewScene(ID("RootID"));
		new_Scene->ID = ID("ID");
		new_Scene->RootID = ID("RootID");
		new_Scene->ActiveCameraID = ID("ActiveCamera");
		new_Scene->Name = STRING("Name");

		SceneManager::SetScene(new_Scene);
		if (BeginStruct("Root"))
		{
			ReadTagComponentIfExists(new_Scene->RootID);
			ReadTransformComponentIfExists(new_Scene->RootID);
			ReadHierachyComponentIfExists(new_Scene->RootID);
			ReadMeshComponentIfExists(new_Scene->RootID);
			ReadSpotlightComponentIfExists(new_Scene->RootID);
			ReadPointlightComponentIfExists(new_Scene->RootID);
			ReadSunlightComponentIfExists(new_Scene->RootID);
			ReadRigidbodyComponentIfExists(new_Scene->RootID);
			ReadColliderComponentIfExists(new_Scene->RootID);
			ReadScriptComponentIfExists(new_Scene->RootID);
			ReadAudioSourceComponentIfExists(new_Scene->RootID);
			ReadSkyboxComponentIfExists(new_Scene->RootID);
			ReadLightProbeComponentIfExists(new_Scene->RootID);
			ReadCharacterControllerComponentIfExists(new_Scene->RootID);
			ReadParticleSystemComponentIfExists(new_Scene->RootID);
			EndStruct();
		}

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
							ReadAudioSourceComponentIfExists(entity_id);
							ReadSkyboxComponentIfExists(entity_id);
							ReadLightProbeComponentIfExists(entity_id);
							ReadAnimationComponentIfExists(entity_id);
							ReadCharacterControllerComponentIfExists(entity_id); 
							ReadParticleSystemComponentIfExists(entity_id);
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

		if (BeginCollection("Prefabs"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Prefab"))
				{
					ScenePrefab prefab;
					prefab.PrefabID = ID("Prefab_ID");
					prefab.SceneID = ID("Scene_ID");
					prefab.ParentID = ID("Parent_ID");
					prefab.Position = VEC3("Position");
					prefab.Rotation = glm::quat(glm::radians(VEC3("Rotation")));
					SceneManager::Instantiate(prefab.SceneID, prefab.Position);
				}
				NextInCollection();
			}
			EndCollection();
		}


		EndStruct();

		return new_Scene;
	}

}

void Fracture::SceneSerialiser::ReadScenePrefab(ScenePrefab prefab)
{
	if (BeginStruct("Scene"))
	{
	
		if (BeginStruct("Root"))
		{
			ReadTagComponentIfExists(prefab.PrefabID);
			ReadTransformComponentIfExists(prefab.PrefabID,prefab.Position,prefab.Scale,prefab.Rotation);
			ReadHierachyComponentIfExists(prefab.PrefabID,prefab.ParentID);
			InstanceMeshComponentIfExists(prefab.PrefabID, prefab.PrefabID,prefab.SceneID);		
			ReadSpotlightComponentIfExists(prefab.PrefabID);
			ReadPointlightComponentIfExists(prefab.PrefabID);
			ReadCameraComponentIfExists(prefab.PrefabID);
			ReadSunlightComponentIfExists(prefab.PrefabID);
			ReadRigidbodyComponentIfExists(prefab.PrefabID);
			ReadColliderComponentIfExists(prefab.PrefabID);
			ReadScriptComponentIfExists(prefab.PrefabID);
			ReadAudioSourceComponentIfExists(prefab.PrefabID);
			ReadSkyboxComponentIfExists(prefab.PrefabID);
			ReadLightProbeComponentIfExists(prefab.PrefabID);
			ReadAnimationComponentIfExists(prefab.PrefabID);
			ReadCharacterControllerComponentIfExists(prefab.PrefabID);
			ReadParticleSystemComponentIfExists(prefab.PrefabID);

			if (BeginCollection("Scripts"))
			{
				while (CurrentCollectionIndex() < GetCollectionSize())
				{
					if (BeginStruct("LuaScript"))
					{
						auto script_id = ID("ScriptID");
						SceneManager::AttachScript(prefab.PrefabID, script_id);
						ScriptManager::LoadScriptProperties(script_id);

						/*
						const auto& luascript = ScriptManager::GetLuaScript(script_id);
						if (BeginCollection("Properties"))
						{
							//auto properties = luascript->GetProperties();
							while (CurrentCollectionIndex() < GetCollectionSize())
							{
								if (BeginStruct("Property"))
								{
									switch ((PROPERTY_TYPE)INT("Type"))
									{
									case PROPERTY_TYPE::UUID:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::UUID;
										prop->ID = ID("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::BOOL:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::BOOL;
										prop->Bool = BOOL("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::COLOR3:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::COLOR3;
										prop->Color3 = VEC3("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::COLOR4:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::COLOR4;
										prop->Color4 = VEC4("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::FLOAT:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::FLOAT;
										prop->Float = FLOAT("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::INT:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::INT;
										prop->Int = INT("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									case PROPERTY_TYPE::STRING:
									{
										auto prop = std::make_shared<ScriptProperty>();
										prop->Name = STRING("Name");
										prop->Type = PROPERTY_TYPE::STRING;
										prop->String = STRING("Value");
										luascript->m_Properties[prop->Name] = prop;
										break;
									}
									}
									EndStruct();
								}
								NextInCollection();
							}
							EndCollection();
						}
						*/
						EndStruct();
					}
					NextInCollection();
				}
				EndCollection();
			}

			EndStruct();
		}

		if (BeginCollection("Entities"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Entity"))
				{					
					auto entity = UUID();
					if (BeginCollection("Components"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							ReadTagComponentIfExists(entity);
							ReadTransformComponentIfExists(entity);
							ReadHierachyComponentIfExists(entity, prefab.PrefabID);
							InstanceMeshComponentIfExists(entity, prefab.PrefabID, prefab.SceneID);
							ReadSpotlightComponentIfExists(entity);
							ReadPointlightComponentIfExists(entity);
							ReadCameraComponentIfExists(entity);
							ReadSunlightComponentIfExists(entity);
							ReadRigidbodyComponentIfExists(entity);
							ReadColliderComponentIfExists(entity);
							ReadScriptComponentIfExists(entity);
							ReadAudioSourceComponentIfExists(entity);
							ReadSkyboxComponentIfExists(entity);
							ReadLightProbeComponentIfExists(entity);
							ReadAnimationComponentIfExists(entity);
							ReadCharacterControllerComponentIfExists(entity);
							ReadParticleSystemComponentIfExists(entity);
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
								SceneManager::AttachScript(prefab.PrefabID, script_id);
								auto luascript = ScriptManager::GetLuaScript(script_id);


								if (BeginCollection("Properties"))
								{
									auto properties = luascript->GetProperties();
									while (CurrentCollectionIndex() < GetCollectionSize())
									{
										if (BeginStruct("Property"))
										{
											switch ((PROPERTY_TYPE)INT("Type"))
											{
											case PROPERTY_TYPE::UUID:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::UUID;
												prop->ID = ID("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::BOOL:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::BOOL;
												prop->Bool = BOOL("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR3:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR3;
												prop->Color3 = VEC3("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::COLOR4:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::COLOR4;
												prop->Color4 = VEC4("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::FLOAT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::FLOAT;
												prop->Float = FLOAT("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::INT:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::INT;
												prop->Int = INT("Value");
												properties[prop->Name] = prop;
												break;
											}
											case PROPERTY_TYPE::STRING:
											{
												auto prop = std::make_shared<ScriptProperty>();
												prop->Name = STRING("Name");
												prop->Type = PROPERTY_TYPE::STRING;
												prop->String = STRING("Value");
												properties[prop->Name] = prop;
												break;
											}
											}
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
				}
				NextInCollection();
			}

			EndCollection();
		}
		
		
		if (BeginCollection("Prefabs"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Prefab"))
				{
					ScenePrefab child_prefab;
					child_prefab.PrefabID = UUID();
					child_prefab.SceneID = ID("Scene_ID");
					child_prefab.ParentID = prefab.PrefabID;
					child_prefab.Position = VEC3("Position");
					child_prefab.Scale = VEC3("Scale");
					child_prefab.Rotation = glm::quat(glm::radians(VEC3("Rotation")));
					SceneManager::InstanceSceneFromFile(child_prefab);
				}
				NextInCollection();
			}
			EndCollection();
		}
		

		EndStruct();
	}
}