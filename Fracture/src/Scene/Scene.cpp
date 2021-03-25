#include "Scene.h"
#include "Entity/UUID.h"

std::shared_ptr<Fracture::Entity> Fracture::Scene::active_Camera;
std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::m_entities;

Fracture::Scene::Scene()
{
	//ROOT ENTITY
	m_root = EntityManager::CreateEntity<Entity>();	
	std::shared_ptr<RelationShipComponent> m_root_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(m_root->GetId()));
	ComponentManager::AddComponent<RelationShipComponent>(m_root_rel);
	ComponentManager::AddComponent<TransformComponent>(m_root->GetId(),glm::vec3(0.0f));
	ComponentManager::AddComponent<TagComponent>(m_root->GetId(), "Root");
	addEntity(m_root);
}

Fracture::Scene::Scene(const std::string& name):
	Name(name)
{
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

void Fracture::Scene::SetRoot(const std::shared_ptr<Entity>& entity)
{
	m_root = entity;
}

void Fracture::Scene::Destroy(const std::shared_ptr<Entity>& entity)
{
	
	std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());
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

	ComponentManager::RemoveComponentsbyID(entity->GetId());
}

void Fracture::Scene::Destroy(UUID id)
{
	for (auto& entity : m_entities)
	{
		if (entity != nullptr && entity->GetId() == id)
		{
			Destroy(entity);
		}
	}
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Duplicate(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Entity> new_entity = EntityManager::CreateEntity<Entity>();	

	CopyComponentIfExists<TagComponent>(new_entity, entity);	
	CopyComponentIfExists<RelationShipComponent>(new_entity, entity);
	CopyComponentIfExists<TransformComponent>(new_entity, entity);
	CopyComponentIfExists<RenderComponent>(new_entity, entity);
	CopyComponentIfExists<LightComponent>(new_entity, entity);
	CopyComponentIfExists<RigidBodyComponent>(new_entity, entity);
	CopyComponentIfExists<BoxColliderComponent>(new_entity, entity);
	CopyComponentIfExists<ScriptComponent>(new_entity, entity);
	
	return new_entity;
}

void Fracture::Scene::clearScene()
{	
	m_entities.clear();
	ComponentManager::ClearComponents();
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

void Fracture::Scene::setCamera(Fracture::UUID id)
{
	if (ComponentManager::HasComponent<CameraControllerComponent>(id))
	{
		active_Camera = GetEntity(id);
	}
	else
	{
		FRACTURE_ERROR("Entity ID: {} - Has No CameraComponent!");
	}
}

std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::Entities()
{
	return m_entities;
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::GetEntity(UUID id)
{
	
	for (auto& entity : m_entities)
	{
		if (entity->GetId() == id)
		{
			return entity;
		}
	}
	FRACTURE_ERROR("COULD NOT FIND ENTITY {}",id);
	return nullptr;
}


