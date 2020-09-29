#include "ComponentManager.h"
#include "Component.h"
#include "IUpdatable.h"
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
	//std::cout << m_Components.size() << std::endl;

	for (auto& component : m_Components)
	{
		std::shared_ptr<IUPDATABLE> c = std::dynamic_pointer_cast<IUPDATABLE>(component);
		if(c)
			c->onUpdate(dt);
	}

	//Push physics components to Physics system
	//push render Components to Render system
	//push Audio Components to Audio System
	//push Script Components to GameLogic System
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
