#include "Scene.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Component/TransformComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/TagComponent.h"
#include "Component/RenderComponent.h"
#include "Component/LightComponent.h"
#include "Component/EditorNodeComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Logging/Logger.h"
#include <iostream>

std::shared_ptr<Fracture::Entity> Fracture::Scene::active_Camera;
std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::m_entities;

Fracture::Scene::Scene()
{
	//ROOT ENTITY
	m_root = EntityManager::CreateEntity<Entity>();	
	std::shared_ptr<RelationShipComponent> m_root_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(m_root->Id));
	ComponentManager::AddComponent<RelationShipComponent>(m_root_rel);
	ComponentManager::AddComponent<TransformComponent>(m_root->Id,glm::vec3(0.0f));
	ComponentManager::AddComponent<TagComponent>(m_root->Id, "Root");
	addEntity(m_root);
}

Fracture::Scene::~Scene()
{	
	clearScene();
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Root()
{
	return m_root;
}

void Fracture::Scene::Instance(std::shared_ptr<Entity> entity)
{
	std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
	
	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> oldTag = ComponentManager::GetComponent<TagComponent>(entity->Id);
		ComponentManager::AddComponent<TagComponent>(newEntity->Id, "CopyOf"+ oldTag->Name);
	}	

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> oldRel = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		std::shared_ptr<RelationShipComponent> newRel = std::make_shared<RelationShipComponent>(newEntity->Id);
		newRel->SetParent(oldRel->m_parent);
		if (oldRel->hasChildren())
		{
			for (auto child : oldRel->GetChildren())
			{
				Instance(GetEntity(child));
			}
		}
		ComponentManager::AddComponent<RelationShipComponent>(newRel);
	}	

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
	}
	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> oldcomponent = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(newEntity->Id, oldcomponent->model->Name, oldcomponent->material->Name);
		component->SetRenderType(RenderType::Normal);
		ComponentManager::AddComponent<RenderComponent>(component);
	}	

	addEntity(newEntity);
}

void Fracture::Scene::Instance(std::shared_ptr<Entity> entity, glm::vec3 pos)
{
	std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();

	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> oldTag = ComponentManager::GetComponent<TagComponent>(entity->Id);
		ComponentManager::AddComponent<TagComponent>(newEntity->Id, "CopyOf" + oldTag->Name);
	}

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> oldRel = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		std::shared_ptr<RelationShipComponent> newRel = std::make_shared<RelationShipComponent>(newEntity->Id);
		newRel->SetParent(oldRel->m_parent);
		if (oldRel->hasChildren())
		{
			for (auto child : oldRel->GetChildren())
			{
				Instance(GetEntity(child));
			}			
		}
		ComponentManager::AddComponent<RelationShipComponent>(newRel);
	}

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		ComponentManager::AddComponent<TransformComponent>(newEntity->Id, pos);
	}
	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> oldcomponent = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		std::shared_ptr<Material> oldMaterial = AssetManager::getMaterial(oldcomponent->material->Name);
		
		
		std::shared_ptr<Material> material = std::make_shared<Material>(oldMaterial->Name + std::to_string(newEntity->Id),oldMaterial->getShader());
		material->CopyUniforms(oldMaterial->GetUniforms());
		material->CopySampleUniforms(oldMaterial->GetSamplerUniforms());
		AssetManager::AddMaterial(material->Name, material);
		
		std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(newEntity->Id, oldcomponent->model->Name, material->Name);
		component->SetRenderType(RenderType::Normal);
		ComponentManager::AddComponent<RenderComponent>(component);
	}
	addEntity(newEntity);
}

void Fracture::Scene::Instance(std::shared_ptr<Entity> entity, glm::vec3 position, glm::vec3 scale)
{
	std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();

	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> oldTag = ComponentManager::GetComponent<TagComponent>(entity->Id);
		ComponentManager::AddComponent<TagComponent>(newEntity->Id, "CopyOf" + oldTag->Name);
	}

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> oldRel = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		std::shared_ptr<RelationShipComponent> newRel = oldRel;
		newRel->EntityID = newEntity->Id;
		ComponentManager::AddComponent<RelationShipComponent>(newRel);
	}

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		ComponentManager::AddComponent<TransformComponent>(newEntity->Id, position, scale);
	}

	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> oldcomponent = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		std::shared_ptr<RenderComponent> component = oldcomponent;
		component->EntityID = newEntity->Id;
		component->SetRenderType(RenderType::Normal);
		ComponentManager::AddComponent<RenderComponent>(component);

	}

	addEntity(newEntity);
}

void Fracture::Scene::Instance(std::shared_ptr<Entity> entity, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();

	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> oldTag = ComponentManager::GetComponent<TagComponent>(entity->Id);
		ComponentManager::AddComponent<TagComponent>(newEntity->Id, "CopyOf" + oldTag->Name);
	}

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> oldRel = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		std::shared_ptr<RelationShipComponent> newRel = std::make_shared<RelationShipComponent>(newEntity->Id);
		newRel->SetParent(oldRel->m_parent);
		if (oldRel->hasChildren())
		{
			for (auto child : oldRel->GetChildren())
			{
				Instance(GetEntity(child));
			}
		}
		ComponentManager::AddComponent<RelationShipComponent>(newRel);
	}

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		ComponentManager::AddComponent<TransformComponent>(newEntity->Id, position, scale, rotation);
	}

	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> oldcomponent = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(newEntity->Id, oldcomponent->model->Name, oldcomponent->material->Name);
		component->SetRenderType(RenderType::InstancedElementsIndirect);
		ComponentManager::AddComponent<RenderComponent>(component);
	}

	addEntity(newEntity);
}

void Fracture::Scene::addEntity(std::shared_ptr<Entity> entity)
{
	m_entities.push_back(entity);
}

void Fracture::Scene::Destroy(std::shared_ptr<Entity> entity)
{
	
	std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
	if (relationship->GetChildren().size() > 0)
	{
		for (auto& child : relationship->GetChildren())
		{
			Destroy(child);
		}
	}

	auto it = std::find_if(std::begin(m_entities), std::end(m_entities), [entity](std::shared_ptr<Entity>& p) { return p == entity; });
	if (it != m_entities.end())
	{
		m_entities.erase(
			std::remove(std::begin(m_entities),
				std::end(m_entities), entity),
			std::end(m_entities));
	}

	ComponentManager::RemoveComponentsbyID(entity->Id);
}

void Fracture::Scene::Destroy(uint32_t id)
{
	for (auto& entity : m_entities)
	{
		if (entity != nullptr && entity->Id == id)
		{
			Destroy(entity);
		}
	}
}

void Fracture::Scene::clearScene()
{	
	m_entities.clear();
	//ComponentManager::ClearComponents();
	IDManager::ResetIDs();
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::ActiveCamera()
{
	if (active_Camera)
		return active_Camera;
	return nullptr;
}

void Fracture::Scene::setCamera(std::shared_ptr<Entity> camera)
{
	active_Camera = camera;
}

std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::Entities()
{
	return m_entities;
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::GetEntity(uint32_t id)
{
	return std::shared_ptr<Entity>();
	for (auto& entity : m_entities)
	{
		if (entity->Id == id)
		{
			return entity;
		}
	}
	FRACTURE_ERROR("COULD NOT FIND ENTITY {}",id);
	return nullptr;
}

