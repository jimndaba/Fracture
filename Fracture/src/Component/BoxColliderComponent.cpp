#include "BoxColliderComponent.h"
#include "Component/ComponentManager.h"
#include "TransformComponent.h"

Fracture::BoxColliderComponent::BoxColliderComponent(uint32_t id,float _SizeX, float _SizeY, float _SizeZ):Component(id,ComponentType::BoxCollider),X(_SizeX), Y(_SizeY), Z(_SizeZ)
{
	m_boxCollider = new btBoxShape(btVector3(btScalar(X), btScalar(Y), btScalar(Z)));
	glm::vec3 position = ComponentManager::GetComponent<TransformComponent>(id)->Position();
	m_Transform.setIdentity();
	m_Transform.setOrigin(btVector3(position.x, position.y, position.z));
}

Fracture::BoxColliderComponent::~BoxColliderComponent()
{
	delete m_boxCollider;
}

void Fracture::BoxColliderComponent::onStart()
{
}

void Fracture::BoxColliderComponent::setScale(glm::vec3 scale)
{
	m_boxCollider->setLocalScaling(ToBtVec3(scale));
	X = scale.x;
	Y = scale.y;
	Z = scale.z;
}
