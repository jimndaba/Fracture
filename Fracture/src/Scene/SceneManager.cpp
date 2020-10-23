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
	m_scenes.emplace(name, scene);
}

void Fracture::SceneManager::RemoveScene(std::string name)
{
	m_scenes.erase(name);
}

void Fracture::SceneManager::SetScene(std::string name)
{
	m_activeScene = m_scenes[name];
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
