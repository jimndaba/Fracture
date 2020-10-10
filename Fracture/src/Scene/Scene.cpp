#include "Scene.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include "Entity/DirectLight.h"
#include "AssetManager/AssetManager.h"
#include "Component/TransformComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Entity/Camera.h"
#include <iostream>

std::shared_ptr < Fracture::Camera > Fracture::Scene::main_Camera;

Fracture::Scene::Scene()
{
	m_root = EntityManager::CreateEntity<Entity>();
	m_root->name = "Root";

	std::shared_ptr<RelationShipComponent> m_root_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(m_root->Id));
	ComponentManager::AddComponent(m_root_rel);
	ComponentManager::AddComponent<TransformComponent>(m_root->Id,glm::vec3(0.0f));

	main_Camera = EntityManager::CreateEntity<Camera>();
	main_Camera->name = "Main Camera";

	std::shared_ptr<RelationShipComponent> m_camera_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(main_Camera->Id));
	
	m_camera_rel->SetParent(m_root);


	addEntity(m_root);
	addEntity(main_Camera);

}

Fracture::Scene::~Scene()
{
	std::cout << "destroy scene"<< std::endl;	
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

void Fracture::Scene::removeEntity(std::shared_ptr<Entity>  entity)
{
	//m_root->removeChild(entity);
}

void Fracture::Scene::clearScene()
{
	std::cout << "clear scene" << std::endl;
	m_entities.clear();
}

std::shared_ptr<Fracture::Camera> Fracture::Scene::MainCamera()
{
	if (main_Camera)
		return main_Camera;
	return nullptr;
}

std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Scene::Entities()
{
	return m_entities;
}

