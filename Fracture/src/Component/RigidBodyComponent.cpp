#include "RigidBodyComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"


Fracture::RigidBodyComponent::RigidBodyComponent(uint32_t id,float _mass):Component(id,ComponentType::Rigidbody),Mass(_mass)
{
	m_Transform.setIdentity();
	glm::vec3 position = ComponentManager::GetComponent<TransformComponent>(id)->Position;
	m_Transform.setOrigin(btVector3(position.x, position.y, position.z));

	collisionGroup = CollisionGroup::COLGROUP_NONE;
	collisionMask = CollisionMask::None;

	btCollisionShape* boxCollider = ComponentManager::GetComponent<BoxColliderComponent>(id)->m_boxCollider;

	btScalar mass(Mass);

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if(isDynamic)
		boxCollider->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass, myMotionState, boxCollider, localInertia);

	rbInfo.m_restitution = 1.0f;

	m_rigid = new btRigidBody(rbInfo);	
	m_rigid->activate();
	m_rigid->setUserPointer(this);

}

Fracture::RigidBodyComponent::~RigidBodyComponent()
{
}

void Fracture::RigidBodyComponent::onStart()
{
}

void Fracture::RigidBodyComponent::setVelocity(glm::vec3 velocity)
{
	m_rigid->setActivationState(1);
	m_rigid->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
}

void Fracture::RigidBodyComponent::Translate(glm::vec3 position)
{
	m_rigid->setActivationState(1);
	m_rigid->translate(btVector3(position.x, position.y, position.z));

}
