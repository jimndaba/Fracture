#include "ComponentManager.h"
#include "Component.h"
#include <iostream>

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::m_Components;


Fracture::ComponentManager::ComponentManager()
{
}

Fracture::ComponentManager::~ComponentManager()
{
	m_Components.clear();
}

void Fracture::ComponentManager::onUpdate()
{
	std::cout << m_Components.size() << std::endl;
	//Push physics components to Physics system
	//push render Components to Render system
	//push Audio Components to Audio System
	//push Script Components to GameLogic System
}
