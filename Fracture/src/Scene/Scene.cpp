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
std::shared_ptr<Fracture::Material> samusMaterial;

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
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//models
	AssetManager::AddModel("samus", "bin/content/models/samus/samus.fbx");
	AssetManager::AddModel("cube", "bin/content/models/cube.obj");
	
	//textures
	AssetManager::AddTexture("container", "bin/content/textures/container.png", TextureType::Diffuse);
	AssetManager::AddTexture("specular", "bin/content/textures/container_specular.png", TextureType::Specular);
	
	AssetManager::AddShader("default", "bin/content/shaders/model/vertex.glsl", "bin/content/shaders/model/fragment.glsl");
	
	std::shared_ptr<Fracture::Material> defaultMaterial = std::shared_ptr<Fracture::Material>(new Material("default", AssetManager::getShader("default")));

	std::shared_ptr<Fracture::Material> samusMaterial = std::shared_ptr<Fracture::Material>(new Material("samus", AssetManager::getShader("default")));

	defaultMaterial->SetTexture("material.diffuse", AssetManager::getTexture("container"), 0);
	defaultMaterial->SetTexture("material.specular", AssetManager::getTexture("specular"), 1);
	
	
	defaultMaterial->setVec3("light.direction", -0.2f, -1.0f, -0.3f); 
	defaultMaterial->setVec3("light.ambient", 0.6f,0.6f,0.6f);
	defaultMaterial->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	defaultMaterial->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	samusMaterial->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	samusMaterial->setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
	samusMaterial->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	samusMaterial->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	// material properties
	defaultMaterial->setFloat("material.shininess", 64.0f);


	AssetManager::AddMaterial("default", defaultMaterial);
	AssetManager::AddMaterial("samus", samusMaterial);

	for (unsigned int i = 0; i < 10; i++)
	{
		std::shared_ptr<Fracture::Entity> cube = EntityManager::Create_Entity_ptr();
		ComponentManager::AddComponent<TransformComponent>(cube->Id, cubePositions[i]);
		ComponentManager::AddComponent<RenderComponent>(cube->Id, "cube", "default");
		addEntity(cube);
	}

	monkey = EntityManager::Create_Entity_ptr();
	ComponentManager::AddComponent<TransformComponent>(monkey->Id, glm::vec3(0.0f), glm::vec3(0.3f));
	ComponentManager::AddComponent<RenderComponent>(monkey->Id, "samus", "samus");
	addEntity(monkey);
}

