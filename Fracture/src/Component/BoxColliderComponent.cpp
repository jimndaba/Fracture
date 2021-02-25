#include "BoxColliderComponent.h"
#include "Component/ComponentManager.h"
#include "TransformComponent.h"

Fracture::BoxColliderComponent::BoxColliderComponent(UUID id,float _SizeX, float _SizeY, float _SizeZ):Component(id),X(_SizeX), Y(_SizeY), Z(_SizeZ)
{
	m_boxCollider =std::make_shared<btBoxShape>(btVector3(btScalar(X), btScalar(Y), btScalar(Z)));
	glm::vec3 position = ComponentManager::GetComponent<TransformComponent>(id)->Position();
	m_Transform.setIdentity();
	m_Transform.setOrigin(btVector3(position.x, position.y, position.z));
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

void Fracture::BoxColliderComponent::Accept(ISceneProbe* visitor)
{
}

nlohmann::json Fracture::BoxColliderComponent::serialise(const std::shared_ptr<ComponentSerialiser>& visitor)
{
	return visitor->visitBoxColliderComponent(*this);
}
