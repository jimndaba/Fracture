#include "RigidBodyComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

Fracture::RigidBodyComponent::RigidBodyComponent(uint32_t id,float _mass):Component(id,ComponentType::Physics),Mass(_mass)
{
	m_Transform.setIdentity();
	glm::vec3 position = ComponentManager::GetComponent<TransformComponent>(id)->Position;
	m_Transform.setOrigin(btVector3(position.x, position.y, position.z));

	btCollisionShape* boxCollider = ComponentManager::GetComponent<BoxColliderComponent>(id)->m_boxCollider;

	btScalar mass(Mass);

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if(isDynamic)
		boxCollider->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxCollider, localInertia);
	m_rigid = new btRigidBody(rbInfo);	
	m_rigid->setUserPointer(this);

}

Fracture::RigidBodyComponent::~RigidBodyComponent()
{
}

void Fracture::RigidBodyComponent::onStart()
{
}
