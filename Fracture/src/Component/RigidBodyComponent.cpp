#include "RigidBodyComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


Fracture::RigidBodyComponent::RigidBodyComponent(uint32_t id,const float& _mass):Component(id,ComponentType::Rigidbody),Mass(_mass)
{
	m_Transform = std::make_shared<btTransform>();
	m_Transform->setIdentity();
	glm::vec3 position = ComponentManager::GetComponent<TransformComponent>(id)->Position();
	m_Transform->setOrigin(btVector3(position.x, position.y, position.z));

	collisionGroup = CollisionGroup::COLGROUP_NONE;
	collisionMask = CollisionMask::None;

	boxCollider = ComponentManager::GetComponent<BoxColliderComponent>(id)->m_boxCollider;
	btScalar mass(Mass);

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if(isDynamic)
		boxCollider->calculateLocalInertia(mass, localInertia);

	myMotionState = std::make_shared<btDefaultMotionState>(*m_Transform.get());
	btRigidBody::btRigidBodyConstructionInfo rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass, myMotionState.get(), boxCollider.get(), localInertia);

	rbInfo.m_restitution = 1.0f;

	m_rigid = std::make_shared<btRigidBody>(rbInfo);	
	m_rigid->activate();
	m_rigid->setUserPointer(this);

}


void Fracture::RigidBodyComponent::onStart()
{
}

void Fracture::RigidBodyComponent::setMass(const float& mass)
{
	Mass = mass;
}

void Fracture::RigidBodyComponent::setVelocity(const glm::vec3& velocity)
{
	if(m_rigid)
	{ 
		m_rigid->setActivationState(1);
		m_rigid->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	}
}

void Fracture::RigidBodyComponent::setPosition(const glm::vec3& pos)
{
	if (m_rigid)
	{
		btTransform& worldTrans = m_rigid->getWorldTransform();
		worldTrans.setOrigin(ToBtVec3(pos));
	}
}

void Fracture::RigidBodyComponent::setRotation(const glm::vec3& rot)
{
	if (m_rigid)
	{
		btTransform& worldTrans = m_rigid->getWorldTransform();
		btQuaternion qt;
		worldTrans.setRotation(ToBtQuat(rot));
	}
}

void Fracture::RigidBodyComponent::Translate(const glm::vec3& position)
{
	m_rigid->setActivationState(1);
	m_rigid->translate(btVector3(position.x, position.y, position.z));
}

void Fracture::RigidBodyComponent::Accept(ISceneProbe* visitor)
{
}
