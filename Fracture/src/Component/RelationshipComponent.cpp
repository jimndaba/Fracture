#include "RelationshipComponent.h"
#include "Component/ComponentManager.h"
#include "Entity/Entity.h"

Fracture::RelationShipComponent::RelationShipComponent(UUID id):Component(id)
{
}

void Fracture::RelationShipComponent::onStart()
{
}

void Fracture::RelationShipComponent::SetParent(const UUID& parent){

	hasParent = true;
	m_parent = parent;
	auto& component = ComponentManager::GetComponent<RelationShipComponent>(parent);
	component->AddChild(GetID());
}

void Fracture::RelationShipComponent::ChangeParent(const UUID& parent)
{
	auto& component = ComponentManager::GetComponent<RelationShipComponent>(m_parent);
	component->RemoveChild(GetID());
	m_parent = parent;
	auto& newcomponent = ComponentManager::GetComponent<RelationShipComponent>(parent);
	newcomponent->AddChild(GetID());
}

void Fracture::RelationShipComponent::AddChild(const UUID& child)
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

void Fracture::RelationShipComponent::RemoveChild(const UUID& child)
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

std::vector<Fracture::UUID> Fracture::RelationShipComponent::GetChildren()
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

void Fracture::RelationShipComponent::Accept(ISceneProbe* visitor)
{
}
