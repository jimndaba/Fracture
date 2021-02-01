#include "TagComponent.h"
#include "Entity/IDManager.h"

Fracture::TagComponent::TagComponent(uint32_t entityID) :Component(entityID,ComponentType::Tag)
{
}

Fracture::TagComponent::TagComponent(uint32_t entityID,const std::string& name) : Component(entityID, ComponentType::Tag),
Name(name)
{
}

void Fracture::TagComponent::onStart()
{
}

void Fracture::TagComponent::SetName(const std::string& name)
{
	Name = name;
}

void Fracture::TagComponent::Accept(ISceneProbe* visitor) 
{
}
