#include "Scene.h"
#include "Entity/Entity.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"


Fracture::Scene::Scene()
{
	m_root = std::shared_ptr<Entity>(new Entity(1));
	ComponentManager::AddComponent(std::shared_ptr<TransformComponent>(new TransformComponent(m_root->Id)));

}

Fracture::Scene::~Scene()
{
}

std::shared_ptr<Fracture::Entity> Fracture::Scene::Root()
{
	return m_root;
}

void Fracture::Scene::addEntity(std::shared_ptr<Entity> entity)
{
	m_root->addChild(entity);
}

void Fracture::Scene::removeEntity(std::shared_ptr<Entity> entity)
{
	m_root->removeChild(entity);
}

void Fracture::Scene::clearScene()
{
	m_root->clearChildren();
}
