#include "TransformComponent.h"
#include "ComponentManager.h"
#include "Component/RelationshipComponent.h"
#include "Entity/Entity.h"
#include <glm\gtx\quaternion.hpp>

Fracture::TransformComponent::TransformComponent(uint32_t entityID):Component(entityID,ComponentType::Transform)
{
	m_Position = glm::vec3(0.0f,0.0f,0.0f);
	m_Scale = glm::vec3(1.0f);
	m_Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(uint32_t entityID, glm::vec3 pos): Component(entityID, ComponentType::Transform),m_Position(pos)
{
	m_Scale = glm::vec3(1.0f);
	m_Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(uint32_t entityID, glm::vec3 pos,glm::vec3 scale) : Component(entityID, ComponentType::Transform)
,m_Position(pos),m_Scale(scale)
{
	m_Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(uint32_t entityID, glm::vec3 pos, glm::vec3 scale,glm::vec3 rotation) : Component(entityID, ComponentType::Transform)
, m_Position(pos), m_Scale(scale),m_Rotation(rotation)
{
}

Fracture::TransformComponent::~TransformComponent()
{
}

void Fracture::TransformComponent::onStart()
{
}

void Fracture::TransformComponent::setPosition(const glm::vec3& value)
{
	m_Position = value;
}

void Fracture::TransformComponent::setScale(const glm::vec3& value)
{
	m_Scale = value;
}

void Fracture::TransformComponent::setRotation(const glm::vec3& value)
{
	m_Rotation = value;
}

glm::vec3 Fracture::TransformComponent::Position()const
{
	return m_Position;
}

glm::vec3 Fracture::TransformComponent::Scale()const
{
	return m_Scale;
}

glm::vec3 Fracture::TransformComponent::Rotation()const
{
	return m_Rotation;
}

glm::mat4 Fracture::TransformComponent::GetLocalTranform()
{
	glm::quat rotQuat = glm::quat(m_Rotation);
	glm::mat4 m_rotation, m_translation, m_scale;
	m_translation = glm::translate(m_Position);
	m_scale = glm::scale(m_Scale);
	m_rotation = glm::toMat4(rotQuat);
	m_LocalTransform = m_translation * m_rotation * m_scale;
	return m_LocalTransform;
}

glm::mat4 Fracture::TransformComponent::GetWorldTransform()
{
	auto component = ComponentManager::GetComponent<RelationShipComponent>(EntityID);

	if (component->hasParent)
	{
		auto m_parentTransform = ComponentManager::GetComponent<TransformComponent>(component->m_parent);
		m_WorldTransform = m_parentTransform->GetWorldTransform() * GetLocalTranform();
	}
	else
	{
		m_WorldTransform = GetLocalTranform();
	}
	return m_WorldTransform;
}

void Fracture::TransformComponent::Accept(ISceneProbe* visitor)
{
}

