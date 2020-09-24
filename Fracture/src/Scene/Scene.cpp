#include "Scene.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include <iostream>

std::shared_ptr<Fracture::Entity> monkey;
std::shared_ptr<Fracture::Material> defaultMaterial;

Fracture::Scene::Scene()
{
	m_root = EntityManager::Create_Entity_ptr();
	m_root->name = "Root";
	ComponentManager::AddComponent<TransformComponent>(m_root->Id,glm::vec3(0.0f));

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

void Fracture::Scene::onLoad()
{
	//models
	//AssetManager::AddModel("monkey", "bin/content/models/Survival_BackPack_2.fbx");
	AssetManager::AddModel("cube", "bin/content/models/cube.obj");
	
	//textures
	AssetManager::AddTexture("container", "bin/content/textures/container.png");

	AssetManager::AddShader("default", "bin/content/shaders/model/vertex.glsl", "bin/content/shaders/model/fragment.glsl");
	

	std::shared_ptr<Fracture::Material> defaultMaterial = std::shared_ptr<Fracture::Material>(new Material("default", AssetManager::getShader("default")));

	defaultMaterial->SetTexture("material.Diffuse", AssetManager::getTexture("container"),0);

	AssetManager::AddMaterial("default", defaultMaterial);

	monkey = EntityManager::Create_Entity_ptr();
	monkey->name = "cube";
	ComponentManager::AddComponent<TransformComponent>(monkey->Id, glm::vec3(0.0f, 0.0f, 0.0f));
	ComponentManager::AddComponent<RenderComponent>(monkey->Id, "cube", "default");
	addEntity(monkey);
}
