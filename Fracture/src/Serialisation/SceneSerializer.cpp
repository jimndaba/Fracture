#include "SceneSerializer.h"
#include "Entity/EntityManager.h"
#include "Component/ComponentManager.h"
#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/RenderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"


Fracture::SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene) : m_scene(scene)
{
}

Fracture::SceneSerializer::~SceneSerializer()
{
}

void Fracture::SceneSerializer::Serialize(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["Scene"] = "Untitled";

	json serialised_entities = json::array_t();
	for (auto& entity : m_scene->Entities())
	{		
		serialised_entities.push_back(SerializeEntity(entity));
	}
	j["Entities"] = serialised_entities;

	m_OutputStream << j;
	m_OutputStream.close();
}

bool Fracture::SceneSerializer::DeSerialize(const std::string& filepath)
{	
	std::ifstream stream(filepath);
	json input;

	if (!stream.good())
	{
		FRACTURE_ERROR("Can't read file");
		return false;
	}	

	stream >> input;
	if (input.is_null())
	{
		FRACTURE_ERROR("File is either non-json file or corrupt;");
		return false;
	}

	FRACTURE_TRACE("Fracture DeSerializing Scene");
	if (!exists(input,"Scene"))
	{
		FRACTURE_ERROR("No Fracture Scene in file!");
		return false;
	}

	if (exists(input, "Entities"))
	{
		for (auto entity : input["Entities"])
		{
			std::cout << entity["Entity ID"];
			DeSerializeEntity(entity);
		}
	}	
	return true;
}

nlohmann::json Fracture::SceneSerializer::SerializeEntity(std::shared_ptr<Entity> entity)
{
	json j;
	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity->Id);		
		json c;
		c["Entity Name"] = tag->Name;
		j["Tag Component"] = c;
	}

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> component = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		json c;		
		c["Has Parent"] = component->hasParent;
		c["Parent ID"] = component->m_parent ;
		c["No. Children"] = component->noChildren ;
		if (component->hasChildren())
		{
			json serialised_children = json::array_t();
			for (auto& child : component->GetChildren())
			{				
				json b;
				b["Child ID"] = child;
				serialised_children.push_back(b);
			}
			c["Children"]=  serialised_children;
		}

		j["Relationship Component"] = c;
	}

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		std::shared_ptr<TransformComponent> component = ComponentManager::GetComponent<TransformComponent>(entity->Id);
		json c;	
		
		c["Position"] = { component->Position.x,component->Position.y,component->Position.z } ;
		c[ "Scale"] = { component->Scale.x,component->Scale.y ,component->Scale.z } ;
		c["Rotation"] =  { component->Rotation.x,component->Rotation.y ,component->Rotation.z } ;		
		j["Transform Component"] = c;
	}

	if (ComponentManager::HasComponent<CameraControllerComponent>(entity->Id))
	{
		std::shared_ptr<CameraControllerComponent> component = ComponentManager::GetComponent<CameraControllerComponent>(entity->Id);
		json c;
		c["Position"] = { component->Position.x,component->Position.y,component->Position.z };
		c["FOV"] = component->foV ;
		c["Near Clip"] = component->nearClip ;
		c["Far Clip"] =  component->farClip ;
		j["Camera Component"] = c;
	}

	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> component = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		json c;
		c["Material"] = component->material->Name;
		c["Shader"]= component->material->getShader()->Name ;
		c["Model"] =component->model->Name ;
		j["Render Component"] = c;
	}

	if (ComponentManager::HasComponent<RigidBodyComponent>(entity->Id))
	{
		std::shared_ptr<RigidBodyComponent> component = ComponentManager::GetComponent<RigidBodyComponent>(entity->Id);
		json c;		
		c["Mass"] =  component->Mass ;
		j["Rigidbody Component"] = c;
	}

	if (ComponentManager::HasComponent<BoxColliderComponent>(entity->Id))
	{
		std::shared_ptr<BoxColliderComponent> component = ComponentManager::GetComponent<BoxColliderComponent>(entity->Id);
		json c;		
		c["Dimensions"] = {component->X,component->Y,component->Z };
		j["Box Collider Component"] =c; 
	}

	j["Entity ID"] = entity->Id;
	return j;
}

void Fracture::SceneSerializer::DeSerializeEntity(nlohmann::json j)
{
	if (exists(j, "Tag Component"))
	{		
		std::shared_ptr<Entity> entity = EntityManager::CreateEntity(j["Entity ID"]);

		auto tagComponent = j["Tag Component"];	
		std::shared_ptr<TagComponent> component = std::make_shared<TagComponent>(entity->Id);
		component->SetName(tagComponent["Entity Name"]);
		ComponentManager::AddComponent(component);			

		if (exists(j, "Relationship Component"))
		{
			auto relationshipComponent = j["Relationship Component"];
			std::shared_ptr<RelationShipComponent> component = std::make_shared<RelationShipComponent>(entity->Id);

			bool hasParent = relationshipComponent["Has Parent"];
			if (hasParent)
			{
				std::cout << "Setting parent to: "<< relationshipComponent["Parent ID"] << "for Entity : " << entity->Id<< std::endl;
				uint32_t id = relationshipComponent["Parent ID"];
				component->SetParent(id);
			}
			
			auto children = relationshipComponent["Children"];

			for (auto child : children)
			{
				component->AddChild(child["Child ID"].get<uint32_t>());
			}

			ComponentManager::AddComponent(component);
		}

		if (exists(j, "Camera Component"))
		{
			auto cameraComponent = j["Camera Component"];
			float fov = cameraComponent["FOV"];
			float Near = cameraComponent["Near Clip"];
			float Far = cameraComponent["Far Clip"];
			std::array<float, 3> pos = cameraComponent["Position"];

			std::shared_ptr<CameraControllerComponent> component = std::make_shared<CameraControllerComponent>(entity->Id);
			component->foV = fov;
			component->farClip = Far;
			component->nearClip = Near;
			component->Position = glm::vec3(pos[0], pos[1], pos[2]);
			ComponentManager::AddComponent(component);
		}
		
		if (exists(j, "Transform Component"))
		{
			auto transformComponent = j["Transform Component"];
			std::array<float, 3> pos = transformComponent["Position"];
			std::array<float, 3> sc = transformComponent["Scale"];
			std::array<float, 3>rot = transformComponent["Rotation"];

			std::shared_ptr<TransformComponent> component = std::make_shared<TransformComponent>(entity->Id);
			component->Position = glm::vec3(pos[0], pos[1], pos[2]);
			component->Scale = glm::vec3(sc[0], sc[1], sc[2]);
			component->Rotation = glm::vec3(rot[0], rot[1], rot[2]);
			ComponentManager::AddComponent(component);

		}

		if (exists(j, "Render Component"))
		{
			auto renderComponent = j["Render Component"];
			std::string model = renderComponent["Model"];
			std::string material = renderComponent["Material"];

			std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(entity->Id,model,material);
			ComponentManager::AddComponent(component);

		}
		
		if (exists(j, "Box Collider Component"))
		{
			auto boxColliderComponent = j["Box Collider Component"];
			std::array<float, 3>dimensions = boxColliderComponent["Dimensions"];

			std::shared_ptr<BoxColliderComponent> component = std::make_shared<BoxColliderComponent>(entity->Id,
				dimensions[0],
				dimensions[1],
				dimensions[2]);
			ComponentManager::AddComponent(component);
		}

		if (exists(j, "Rigidbody Component"))
		{
			auto rigidbodyComponent = j["Rigidbody Component"];
			int mass = rigidbodyComponent["Mass"];
			std::shared_ptr<RigidBodyComponent> component = std::make_shared<RigidBodyComponent>(entity->Id, mass);
			ComponentManager::AddComponent(component);
		}
		
		

		m_scene->addEntity(entity);
	}

}
