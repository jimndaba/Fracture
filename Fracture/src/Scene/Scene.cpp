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
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Logging/Logger.h"
#include <iostream>

std::shared_ptr < Fracture::Entity > Fracture::Scene::main_Camera;

Fracture::Scene::Scene()
{
	m_root = EntityManager::CreateEntity<Entity>();
	m_root->name = "Root";

	std::shared_ptr<RelationShipComponent> m_root_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(m_root->Id));
	ComponentManager::AddComponent(m_root_rel);
	ComponentManager::AddComponent<TransformComponent>(m_root->Id,glm::vec3(0.0f));

	main_Camera = EntityManager::CreateEntity<Entity>();
	main_Camera->name = "Main Camera";
	std::shared_ptr<RelationShipComponent> m_camera_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(main_Camera->Id));	
	m_camera_rel->SetParent(m_root);
	ComponentManager::AddComponent(m_camera_rel);
	ComponentManager::AddComponent<CameraControllerComponent>(main_Camera->Id);

	addEntity(m_root);
	addEntity(main_Camera);

}

Fracture::Scene::~Scene()
{
	
	clearScene();
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Root()
{
	return m_root;
}

void Fracture::Scene::addEntity(std::shared_ptr<Entity> entity)
{
	m_entities.push_back(entity);
}

void Fracture::Scene::Destroy(std::shared_ptr<Entity> entity)
{
}

void Fracture::Scene::clearScene()
{	
	m_entities.clear();
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::MainCamera()
{
	if (main_Camera)
		return main_Camera;
	return nullptr;
}

std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::Entities()
{
	return m_entities;
}

