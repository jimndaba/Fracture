#include "TagComponent.h"
#include "Entity/IDManager.h"

Fracture::TagComponent::TagComponent(int entityID) :Component(entityID,ComponentType::Tag)
{
}

Fracture::TagComponent::~TagComponent()
{
}

void Fracture::TagComponent::SetName(std::string name)
{
	Name = name;
}
