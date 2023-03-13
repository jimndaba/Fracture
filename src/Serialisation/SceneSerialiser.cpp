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
		Property("ChildID", child);
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
		transform->Rotation = glm::quat(VEC3("Rotation"));
		SceneManager::AddComponentByInstance<TransformComponent>(entity_id, transform);
		EndStruct();
	}
}

void Fracture::SceneSerialiser::ReadHierachyComponentIfExists(Fracture::UUID entity_id)
{
	if (BeginStruct("Hierachy"))
	{
		auto comp = std::make_shared<HierachyComponent>(entity_id);
		comp->Parent = ID("Parent");
		comp->HasParent = BOOL("Parent");
		BeginCollection("Children");
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			comp->Children.push_back(ID("ChildID"));
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