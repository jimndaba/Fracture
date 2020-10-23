#include "EnemyScript.h"

Fracture::EnemyScript::EnemyScript(uint32_t id):m_id(id)
{
	
}

void Fracture::EnemyScript::onStart()
{
	transform = ComponentManager::GetComponent<TransformComponent>(m_id);
	rigidbody = ComponentManager::GetComponent<RigidBodyComponent>(m_id);
	rigidbody->setVelocity(glm::vec3(0, -EnemySpeed, 0));
	_desiredVelocity = rigidbody->m_rigid->getLinearVelocity().length();
	position = transform->Position;
}

void Fracture::EnemyScript::onUpdate(float dt)
{
	btVector3 currentVelocityDirection = rigidbody->m_rigid->getLinearVelocity();
	btScalar currentVelocty = currentVelocityDirection.length();
	if (currentVelocty < _desiredVelocity)
	{
		currentVelocityDirection *= _desiredVelocity / currentVelocty;
		rigidbody->m_rigid->setLinearVelocity(currentVelocityDirection);
	}
}

void Fracture::EnemyScript::onLateUpdate(float dt)
{
}

void Fracture::EnemyScript::onCollision(CollisionEvent* collision)
{
}
