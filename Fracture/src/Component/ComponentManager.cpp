#include "ComponentManager.h"
#include "Component.h"
#include "TagComponent.h"
#include "IUpdatable.h"
#include "Game/Game.h"
#include "Scripting/ScriptManager.h"
#include "ScriptComponent.h"

#include <iostream>
#include "Profiling/Profiler.h"

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::m_Components;


Fracture::ComponentManager::ComponentManager()
{
}

Fracture::ComponentManager::~ComponentManager()
{
	m_Components.clear();
}

void Fracture::ComponentManager::onLoad()
{
	
	for (auto& component : m_Components)
	{
		std::shared_ptr<BoxColliderComponent> c = std::dynamic_pointer_cast<BoxColliderComponent>(component);
		if (c)
			PhysicsManager::AddCollider(c->EntityID,c->m_boxCollider);
	}

	for (auto& component : m_Components)
	{
		std::shared_ptr<RigidBodyComponent> c = std::dynamic_pointer_cast<RigidBodyComponent>(component);
		if (c)
			PhysicsManager::AddRigidBody(c->EntityID,c->m_rigid,c->collisionGroup,c->collisionMask);
	}
	
}

void Fracture::ComponentManager::onUpdate(float dt)
{	
	ProfilerTimer timer("Component OnUpdate");
	for (auto& component : m_Components)
	{
		std::shared_ptr<IUPDATABLE> c = std::dynamic_pointer_cast<IUPDATABLE>(component);
		if (c)
			c->onUpdate(dt);			
	}

	for (auto& component : m_Components)
	{
		std::shared_ptr<ScriptComponent> c = std::dynamic_pointer_cast<ScriptComponent>(component);
		if(c)
			Game::AddScript(c->GetScript());
	}
}

void Fracture::ComponentManager::ClearComponents()
{
	m_Components.clear();
}

void Fracture::ComponentManager::AddComponent(std::shared_ptr<Component> component)
{
	m_Components.push_back(component);
}

void Fracture::ComponentManager::RemoveComponentsbyID(uint32_t id)
{	
	for (auto& component : m_Components) 
	{
		if (component != nullptr && component->EntityID == id)
		{
			FRACTURE_TRACE("Removed component: {} of Type {}", component->EntityID, component->componentType);
			RemoveComponent(component);
		}
	}
}

void Fracture::ComponentManager::RemoveComponent(std::shared_ptr<Component> component)
{
	FRACTURE_INFO("Removing Component {}",component->EntityID);

	if (component->componentType == ComponentType::BoxCollider)
	{
		FRACTURE_INFO("Removing COllider");
		std::shared_ptr<BoxColliderComponent> collider = std::dynamic_pointer_cast<BoxColliderComponent>(component);
		PhysicsManager::RemoveCollider(collider->m_boxCollider);
	}

	if (component->componentType == ComponentType::Rigidbody)
	{
		FRACTURE_INFO("Removing Rigidbody");
		std::shared_ptr<RigidBodyComponent> rigidbody = std::dynamic_pointer_cast<RigidBodyComponent>(component);
		PhysicsManager::RemoveRigidBody(rigidbody->m_rigid);
	}

	auto it = std::find_if(std::begin(m_Components), std::end(m_Components), [component](std::shared_ptr<Component>& p) { return p == component; });

	if (it != m_Components.end())
	{
		m_Components.erase(
			std::remove(std::begin(m_Components),
				std::end(m_Components), component),
			std::end(m_Components));
	}
	
}

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::GetComponents(uint32_t enitytId)
{
	ProfilerTimer timer("Component GetComponents");
	std::vector<std::shared_ptr<Component>> components;

	for (int i = 0; i < m_Components.size(); i++)
	{

		if (m_Components[i] != NULL && m_Components[i]->EntityID == enitytId)
		{
			components.push_back(m_Components[i]);
		}
	}
	return components;
}

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::GetAllComponents()
{
	return m_Components;
}
