#include "FracturePCH.h"
#include "TransformSystem.h"
#include "Components.h"

Fracture::TransformSystem::TransformSystem()
{
}

void Fracture::TransformSystem::Update(float dt, TransformComponent& component)
{	
	glm::mat4 mTransform = glm::mat4(1.0f);
	glm::mat4 m_rotation, m_translation, m_scale;
	m_translation = glm::translate(component.Position);
	m_scale = glm::scale(component.Scale);
	m_rotation = glm::toMat4(component.Rotation);
	component.LocalTransform = m_translation * m_rotation * m_scale;
}
