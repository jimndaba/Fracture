#include "TagComponent.h"

Fracture::TagComponent::TagComponent(UUID entityID) :Component(entityID)
{
}

Fracture::TagComponent::TagComponent(UUID entityID,const std::string& name) : Component(entityID),
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

nlohmann::json Fracture::TagComponent::serialise(const std::shared_ptr<ComponentSerialiser>& visitor)
{
	return visitor->visitTagComponent(*this);
}
