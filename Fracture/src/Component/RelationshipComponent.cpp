#include "RelationshipComponent.h"
#include "Component/ComponentManager.h"
#include "Logging/Logger.h"
#include "Entity/Entity.h"

Fracture::RelationShipComponent::RelationShipComponent(uint32_t id):Component(id,ComponentType::Relationship)
{
}

Fracture::RelationShipComponent::~RelationShipComponent()
{
}

void Fracture::RelationShipComponent::onStart()
{
}

void Fracture::RelationShipComponent::SetParent(uint32_t parent){

	hasParent = true;
	m_parent = parent;
	RelationShipComponent& component = *ComponentManager::GetComponent<RelationShipComponent>(parent);
	component.AddChild(EntityID);
}

void Fracture::RelationShipComponent::ChangeParent(uint32_t parent)
{
	RelationShipComponent& component = *ComponentManager::GetComponent<RelationShipComponent>(m_parent);
	component.RemoveChild(EntityID);
	m_parent = parent;
	RelationShipComponent& newcomponent = *ComponentManager::GetComponent<RelationShipComponent>(parent);
	newcomponent.AddChild(EntityID);
}

void Fracture::RelationShipComponent::AddChild(uint32_t child)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
		{
			FRACTURE_ERROR("ENTITY ALREADY HAS THIS CHILD ADDED: {}", child);
			return;
		}
	}

	noChildren += 1;
	m_children.push_back(child);
}

void Fracture::RelationShipComponent::RemoveChild(uint32_t child)
{
	auto it = std::find_if(std::begin(m_children), std::end(m_children), [child](uint32_t p) { return p == child; });

	if (it != m_children.end())
	{
		m_children.erase(
			std::remove(std::begin(m_children),
				std::end(m_children), child),
			std::end(m_children));
		noChildren -= 1;
	}
}

std::vector<uint32_t> Fracture::RelationShipComponent::GetChildren()
{
	return m_children;
}

bool Fracture::RelationShipComponent::hasChildren()
{
	if (m_children.size() > 0)
	{
		return true;
	}
	return false;
}
