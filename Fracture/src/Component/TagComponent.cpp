#include "TagComponent.h"
#include "Entity/IDManager.h"

Fracture::TagComponent::TagComponent(uint32_t entityID) :Component(entityID,ComponentType::Tag)
{
}

Fracture::TagComponent::TagComponent(uint32_t entityID, std::string name) : Component(entityID, ComponentType::Tag),
Name(name)
{
}

Fracture::TagComponent::~TagComponent()
{
}

void Fracture::TagComponent::onStart()
{
}

void Fracture::TagComponent::SetName(std::string name)
{
	Name = name;
}
