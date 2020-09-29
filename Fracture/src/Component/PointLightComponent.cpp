#include "PointLightComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"

Fracture::PointLightComponent::PointLightComponent(int id):Component(id,ComponentType::PointLight)
{
}

Fracture::PointLightComponent::~PointLightComponent()
{
}

void Fracture::PointLightComponent::SetPosition(glm::vec3 pos)
{
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(this->entityID);
    transform->Position = pos;
}

glm::vec3 Fracture::PointLightComponent::GetPosition()
{    
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(this->entityID);
    if (transform)
        return transform->Position;
    return glm::vec3();
}
