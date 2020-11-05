#include "EditorNodeComponent.h"
#include "ComponentManager.h"
#include <glm\gtx\quaternion.hpp>
#include <Component\RelationshipComponent.h>
#include <Component\TransformComponent.h>

Fracture::EditorNode::EditorNode(uint32_t id):Component(id,ComponentType::Transform)
{
	m_Position = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
	m_Rotation = glm::vec3(0.0f);
}

Fracture::EditorNode::~EditorNode()
{
}

void Fracture::EditorNode::onStart()
{
}

void Fracture::EditorNode::SetPosition(glm::vec3 value)
{
	m_Position = value;
}

void Fracture::EditorNode::SetRotation(glm::vec3 value)
{
	m_Rotation = value;
}

void Fracture::EditorNode::SetScale(glm::vec3 value)
{
	m_Scale = value;
}

glm::vec3 Fracture::EditorNode::GetPosition()
{
	return m_Position;
}

glm::vec3 Fracture::EditorNode::GetRotation()
{
	return m_Rotation;
}

glm::vec3 Fracture::EditorNode::GetScale()
{
	return m_Scale;
}

glm::mat4 Fracture::EditorNode::GetLocalTranform()
{
	glm::quat rotQuat = glm::quat(m_Rotation);
	glm::mat4 m_rotation, m_translation, m_scale;
	m_translation = glm::translate(m_Position);
	m_scale = glm::scale(m_Scale);
	m_rotation = glm::toMat4(rotQuat);
	m_LocalTransform = m_translation * m_rotation * m_scale;
	return m_LocalTransform;
}

glm::mat4 Fracture::EditorNode::GetWorldTransform()
{
	RelationShipComponent& component = *ComponentManager::GetComponent<RelationShipComponent>(EntityID);

	if (component.hasParent)
	{
		TransformComponent& m_parentTransform = *ComponentManager::GetComponent<TransformComponent>(component.m_parent);
		m_WorldTransform = m_parentTransform.GetWorldTransform() * GetLocalTranform();
	}
	else
	{
		m_WorldTransform = GetLocalTranform();
	}
	return m_WorldTransform;
}

