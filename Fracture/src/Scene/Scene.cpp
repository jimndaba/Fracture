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

}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Root()
{
	return m_root;
}

void Fracture::Scene::addEntity(const std::shared_ptr<Entity>& entity)
{
	m_entities.push_back(entity);
}

void Fracture::Scene::Destroy(const std::shared_ptr<Entity>& entity)
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

void Fracture::Scene::Duplicate(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Entity> new_entity = EntityManager::CreateEntity<Entity>();		
	CopyComponentIfExists<RelationShipComponent>(new_entity, entity);
	CopyComponentIfExists<TagComponent>(new_entity, entity);	
	addEntity(new_entity);
}

void Fracture::Scene::clearScene()
{	
	m_entities.clear();
	ComponentManager::ClearComponents();
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


template<class name>
void Fracture::Scene::CopyComponentIfExists(const std::shared_ptr<Entity>& copyTo, const std::shared_ptr<Entity>& copyFrom)
{
	if (ComponentManager::HasComponent<name>(copyFrom->Id))
	{
		const auto& component = ComponentManager::GetComponent<name>(copyFrom->Id);

		switch (component->componentType)
		{
			case ComponentType::Tag:
			{
				std::shared_ptr<TagComponent> newComponent = std::make_shared<TagComponent>(copyTo->Id, "Copy");
				newComponent = std::dynamic_pointer_cast<TagComponent>(component);
				ComponentManager::AddComponent<name>(newComponent);
				FRACTURE_INFO("Add Tag Component for Entity: {}", copyTo->Id);
			};
			case ComponentType::Relationship:
			{
				std::shared_ptr<RelationShipComponent> newComponent = std::make_shared<RelationShipComponent>(copyTo->Id);
				newComponent = std::dynamic_pointer_cast<RelationShipComponent>(component);
				ComponentManager::AddComponent<name>(newComponent);
				FRACTURE_INFO("Add Relationship Component for Entity: {}", copyTo->Id);
			};
		}
	}
}