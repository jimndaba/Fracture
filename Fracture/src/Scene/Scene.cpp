#include "Scene.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include "Entity/DirectLight.h"
#include "AssetManager/AssetManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include <iostream>

Fracture::Scene::Scene()
{
	m_root = EntityManager::CreateEntity<Entity>();
	m_root->name = "Root";
	ComponentManager::AddComponent<TransformComponent>(m_root->Id,glm::vec3(0.0f));
	main_Camera = EntityManager::CreateEntity<Camera>();
	addEntity(main_Camera);

}

Fracture::Scene::~Scene()
{
	std::cout << "destroy scene"<< std::endl;	
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Root()
{
	return m_root;
}

void Fracture::Scene::addEntity(std::shared_ptr<Entity> entity)
{
	m_root->addChild(entity);
}

void Fracture::Scene::removeEntity(std::shared_ptr<Entity>  entity)
{
	m_root->removeChild(entity);
}

void Fracture::Scene::clearScene()
{
	std::cout << "clear scene" << std::endl;
	m_root->clearChildren();
}

std::shared_ptr<Fracture::Camera> Fracture::Scene::MainCamera()
{
	if (main_Camera)
		return main_Camera;
	return nullptr;
}

