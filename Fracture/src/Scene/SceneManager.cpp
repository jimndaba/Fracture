#include "SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Component/ComponentManager.h"
#include "Component/LightComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/RelationshipComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityFactory.h"

std::map<std::string, std::shared_ptr<Fracture::Scene>> Fracture::SceneManager::m_scenes;
std::shared_ptr<Fracture::Scene> Fracture::SceneManager::m_activeScene;

Fracture::SceneManager::SceneManager()
{
}

Fracture::SceneManager::~SceneManager()
{
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::NewScene()
{
	std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

	//CAMERA ENTITY
	std::shared_ptr<Entity> main_Camera = EntityManager::CreateEntity<Entity>();
	std::shared_ptr<RelationShipComponent> m_camera_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(main_Camera->Id));
	m_camera_rel->SetParent(newScene->Root()->Id);
	ComponentManager::AddComponent<RelationShipComponent>(m_camera_rel);
	ComponentManager::AddComponent<CameraControllerComponent>(main_Camera->Id);
	ComponentManager::AddComponent<TagComponent>(main_Camera->Id, "Main Camera");

	//SUNLIGHT ENTITY
	std::shared_ptr<Entity> main_sunlight = EntityManager::CreateEntity<Entity>();
	ComponentManager::AddComponent<TagComponent>(main_sunlight->Id, "Sun");
	std::shared_ptr<RelationShipComponent> sun_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(main_sunlight->Id));
	sun_rel->SetParent(newScene->Root()->Id);
	ComponentManager::AddComponent<RelationShipComponent>(sun_rel);
	std::shared_ptr<EditorNode> editornode = std::make_shared<EditorNode>(main_sunlight->Id);
	editornode->SetRotation(glm::vec3(0.0f,-1.0f,0.0f));
	ComponentManager::AddComponent<EditorNode>(editornode);
	ComponentManager::AddComponent<LightComponent>(main_sunlight->Id, LightType::Sun);

	//SKYLIGHT ENTITY
	std::shared_ptr<Entity> main_skylight = EntityManager::CreateEntity<Entity>();
	ComponentManager::AddComponent<TagComponent>(main_skylight->Id, "SkyLigth");
	std::shared_ptr<RelationShipComponent> sky_rel = std::shared_ptr<RelationShipComponent>(new RelationShipComponent(main_skylight->Id));
	sky_rel->SetParent(newScene->Root()->Id);
	ComponentManager::AddComponent<RelationShipComponent>(sky_rel);
	ComponentManager::AddComponent<LightComponent>(main_skylight->Id, LightType::Sky);

	newScene->addEntity(main_Camera);
	newScene->addEntity(main_sunlight);
	newScene->addEntity(main_skylight);

	newScene->setCamera(main_Camera);

	return newScene;
}

void Fracture::SceneManager::AddScene(std::string name,std::shared_ptr<Scene> scene)
{
	scene->Name = name;
	m_scenes.emplace(name, scene);
}

void Fracture::SceneManager::RemoveScene(std::string name)
{
	m_scenes.erase(name);
}

void Fracture::SceneManager::SetScene(std::string name)
{
	m_activeScene = m_scenes[name];
	m_activeScene->onLoad();
}

void Fracture::SceneManager::SetSceneName(std::shared_ptr<Scene> scene,std::string name)
{	
	auto entry = m_scenes.find(scene->Name);
	if (entry != end(m_scenes))
	{
		auto const value = std::move(entry->second);
		m_scenes.erase(entry);
		m_scenes.insert({ name,std::move(value) });
	}
	m_activeScene->Name = name;
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::GetActiveScene()
{
	return m_activeScene;
}

void Fracture::SceneManager::AddEntity(std::shared_ptr<Entity> entity)
{
	m_activeScene->addEntity(entity);
}

void Fracture::SceneManager::RemoveEntity(std::shared_ptr<Entity> entity)
{
	m_activeScene->Destroy(entity);
}

void Fracture::SceneManager::RemoveEntity(uint32_t entity)
{
	m_activeScene->Destroy(entity);
}

std::shared_ptr<Fracture::Entity> Fracture::SceneManager::getEntity(uint32_t id)
{
	return m_activeScene->GetEntity(id);
}

std::map<std::string, std::shared_ptr<Fracture::Scene>> Fracture::SceneManager::GetScenes()
{
	return m_scenes;
}
