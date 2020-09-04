#include "ComponentManager.h"
#include "Component.h"

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentManager::m_Components;


Fracture::ComponentManager::ComponentManager()
{
}

Fracture::ComponentManager::~ComponentManager()
{
}

void Fracture::ComponentManager::onUpdate()
{
	//Push physics components to Physics system
	//push render Components to Render system
	//push Audio Components to Audio System
	//push Script Components to GameLogic System
}

void Fracture::ComponentManager::AddComponent(std::shared_ptr<Component> component)
{
	component->onAttach();
	m_Components.push_back(component);
}

void Fracture::ComponentManager::RemoveComponent(std::shared_ptr<Component> component)
{
	component->onDettach();
	std::vector<std::shared_ptr<Component>>::iterator new_end;
	new_end = std::remove(m_Components.begin(), m_Components.begin(),component);

}

