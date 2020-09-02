#include "TransformComponent.h"

Fracture::TransformComponent::TransformComponent(int entityID):Component(entityID,ComponentType::Transform)
{
}

Fracture::TransformComponent::~TransformComponent()
{
}

void Fracture::TransformComponent::onAttach()
{
}

void Fracture::TransformComponent::onDettach()
{

}

glm::mat4 Fracture::TransformComponent::GetLocalTranform()
{
	glm::quat rotQuat = glm::quat(Rotation);
	glm::mat4 m_rotation, m_translation, m_scale;
	m_translation = glm::translate(Position);
	m_scale = glm::scale(Scale);
	m_rotation = glm::toMat4(rotQuat);
	m_LocalTransform = m_translation * m_rotation * m_scale;
	return m_LocalTransform;
}

glm::mat4 Fracture::TransformComponent::GetWorldTransform()
{
    return m_LocalTransform;
}

