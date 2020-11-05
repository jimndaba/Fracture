#include "EditorNodeComponent.h"

Fracture::EditorNode::EditorNode(uint32_t id):Component(id,ComponentType::Transform)
{
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

