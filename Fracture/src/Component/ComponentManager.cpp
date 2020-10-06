#include "ComponentManager.h"
#include "Component.h"
#include "TagComponent.h"
#include "IUpdatable.h"
#include "Game/Game.h"
#include "Scripting/ScriptManager.h"
#include "ScriptComponent.h"
#include <iostream>

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::m_Components;


Fracture::ComponentManager::ComponentManager()
{
}

Fracture::ComponentManager::~ComponentManager()
{
	m_Components.clear();
}

void Fracture::ComponentManager::onUpdate(float dt)
{	
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

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::GetComponents(int enitytId)
{
	std::vector<std::shared_ptr<Component>> components;

	for (int i = 0; i < m_Components.size(); i++)
	{

		if (m_Components[i] != NULL && m_Components[i]->entityID == enitytId)
		{
			components.push_back(m_Components[i]);
		}
	}
	return components;
}
