#include "PointLightComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"

Fracture::PointLightComponent::PointLightComponent(uint32_t id):Component(id,ComponentType::PointLight)
{
}

Fracture::PointLightComponent::~PointLightComponent()
{
}

void Fracture::PointLightComponent::onStart()
{
}

void Fracture::PointLightComponent::SetPosition(glm::vec3 pos)
{
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(this->EntityID);
    transform->Position = pos;
}

glm::vec3 Fracture::PointLightComponent::GetPosition()
{    
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(this->EntityID);
    if (transform)
        return transform->Position;
    return glm::vec3();
}
