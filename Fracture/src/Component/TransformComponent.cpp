#include "TransformComponent.h"
#include "ComponentManager.h"

Fracture::TransformComponent::TransformComponent(int entityID):Component(entityID,ComponentType::Transform)
{
	Position = glm::vec3(0.0f,-5.0f,0.0f);
	Scale = glm::vec3(0.5f);
	Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(int entityID, glm::vec3 pos): Component(entityID, ComponentType::Transform)
{
	Position = pos;
	Scale = glm::vec3(0.5f);
	Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(int entityID, glm::vec3 pos,glm::vec3 scale) : Component(entityID, ComponentType::Transform)
{
	Position = pos;
	Scale = scale;
	Rotation = glm::vec3(0.0f);
}

Fracture::TransformComponent::TransformComponent(int entityID, glm::vec3 pos, glm::vec3 scale,glm::vec3 rotation) : Component(entityID, ComponentType::Transform)
{
	Position = pos;
	Scale = scale;
	Rotation = rotation;
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
	/* TODO
	if (this->GetEntityParent())
	{
		m_parentTransform = Game::ComponentManager()->GetComponent<DiTransformComponent>(this->GetEntityParent()->GetID());
		//(DiTransformComponent*)Game::ComponentManager()->GetComponent(ComponentType::Transform, this->GetEntityParent()->GetID());
		m_WorldTransform = m_parentTransform->GetWorldTransform() * GetLocalTranform();
	}
	else
	{
		m_WorldTransform = GetLocalTranform();
	}
	*/
	//return m_WorldTransform;
    return GetLocalTranform();
}

