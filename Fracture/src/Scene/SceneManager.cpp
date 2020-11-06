#include "SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"


std::map<std::string, std::shared_ptr<Fracture::Scene>> Fracture::SceneManager::m_scenes;
std::shared_ptr<Fracture::Scene> Fracture::SceneManager::m_activeScene;

Fracture::SceneManager::SceneManager()
{
}

Fracture::SceneManager::~SceneManager()
{
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
