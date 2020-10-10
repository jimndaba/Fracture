#include "RelationshipComponent.h"
#include "Component/ComponentManager.h"
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

void Fracture::RelationShipComponent::SetParent(std::shared_ptr<Entity> parent){

	m_parent = parent;
	RelationShipComponent& component = *ComponentManager::GetComponent<RelationShipComponent>(parent->Id);
	component.noChildren += 1;
}
