#include "FracturePCH.h"
#include "SceneSerialiser.h"
#include "Common/UUID.h"
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
	BeginStruct("MeshComponent");
	Property("MeshID", component->Mesh);
	Property("MaterialID", component->Material);
	Property("ShaderID", component->Shader);
	Property("MeshType", (int)component->meshType);
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::CameraComponent* component)
{
	BeginStruct("CameraComponent");
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
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::PointlightComponent* component)
{
	BeginStruct("PointlightComponent");
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::SpotlightComponent* component)
{
	BeginStruct("SpotlightComponent");
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::SunlightComponent* component)
{
	BeginStruct("SunlightComponent");
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
	EndStruct();
}

void Fracture::SceneSerialiser::SerialiseComponent(Fracture::ColliderComponent* component)
{
	BeginStruct("ColliderComponent");
	EndStruct();
}


void Fracture::SceneSerialiser::WriteScene(Fracture::Scene* scene)
{
	BeginStruct("Scene");
	{
		Property("RootID", scene->ID);
		// Property("ActiveCamera", mScene.ActiveCamera);

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
		//Application::GetSceneManager()->SetScene(new_Scene);
		//new_Scene->ActiveCamera = ID("ActiveCamera");	

		BeginCollection("Entities");
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			BeginStruct("Entity");
			auto entity_id = ID("Entity_ID");
			SceneManager::AddEntity(entity_id);


			BeginCollection("Components");
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Tag"))
				{
					auto name = STRING("Name");
					SceneManager::AddComponent<TagComponent>(entity_id, name);
					EndStruct();
				}
				/*
				if (BeginStruct("Transform"))
				{
					auto position = VEC3("Position");
					auto scale = VEC3("Scale");
					auto rotation = VEC3("Rotation");
					Application::GetSceneManager()->AddComponent(entity_id, std::make_shared<TransformComponent>(entity_id, position, scale, rotation));
					EndStruct();
				}
				if (BeginStruct("Camera"))
				{
					CameraComponentParams p;
					p.Damping = FLOAT("Damping");
					p.Far = FLOAT("Far");
					p.Near = FLOAT("Near");
					p.Yaw = FLOAT("Yaw");
					p.Pitch = FLOAT("Pitch");
					p.Roll = FLOAT("Roll");
					p.Position = VEC3("Position");
					Application::GetSceneManager()->AddComponent(entity_id, std::make_shared<CameraComponent>(entity_id, p));
					EndStruct();
				}
				if (BeginStruct("Hierachy"))
				{
					HierachyParams p;
					p.HasParent = BOOL("HasParent");

					if (p.HasParent)
					{
						p.Parent = ID("Parent");
					}

					BeginCollection("Children");
					while (CurrentCollectionIndex() < GetCollectionSize())
					{
						if (BeginStruct("Child"))
						{
							auto child_id = ID("Child_ID");
							p.Children.push_back(child_id);
							EndStruct();
						}
						NextInCollection();
					}
					EndCollection();

					Application::GetSceneManager()->AddComponent(entity_id, std::make_shared<HierachyComponent>(entity_id, p));

					EndStruct();
				}
				if (BeginStruct("StaticMesh"))
				{
					auto MeshName = STRING("Mesh");
					auto mesh = AssetManager::GetStaticMesh(MeshName.c_str());
					if (mesh)
					{
						auto component = std::make_shared<StaticMeshComponent>(MeshName, mesh, entity_id);
						component->OverrideMeshMaterial = BOOL("Override Material");
						component->MaterialID = ID("MaterialID");
						Application::GetSceneManager()->AddComponent(entity_id, component);
					}
					EndStruct();
				}
				if (BeginStruct("Pointlight"))
				{
					PointlightParams p;
					p.Diffuse = VEC3("Diffuse");
					p.Specular = VEC3("Specular");
					p.Intensity = FLOAT("Intensity");
					p.Radius = FLOAT("Radius");
					p.Compression = FLOAT("Compression");
					Application::GetSceneManager()->AddComponent(entity_id, std::make_shared<PointlightComponent>(entity_id, p));
					EndStruct();
				}
				*/
				NextInCollection();
			}
			EndCollection();

			EndStruct();
			NextInCollection();
		}

		EndCollection();
		EndStruct();

		return new_Scene;
	}
}