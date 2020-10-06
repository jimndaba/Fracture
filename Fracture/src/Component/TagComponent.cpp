#include "TagComponent.h"
#include "Entity/IDManager.h"

Fracture::TagComponent::TagComponent(int entityID) :Component(entityID,ComponentType::Tag)
{
}

Fracture::TagComponent::TagComponent(int entityID, std::string name) : Component(entityID, ComponentType::Tag),
Name(name)
{
}

Fracture::TagComponent::~TagComponent()
{
}

void Fracture::TagComponent::onAttach()
{
}

void Fracture::TagComponent::onDettach()
{
}

void Fracture::TagComponent::SetName(std::string name)
{
	Name = name;
}
