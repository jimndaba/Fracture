#include "BulletScript.h"

Fracture::BulletScript::BulletScript(uint32_t id, glm::vec3 position):GameLogic(),m_id(id),m_position(position)
{
}

Fracture::BulletScript::~BulletScript()
{
}

void Fracture::BulletScript::onStart()
{
	rigidbody = GetComponent<RigidBodyComponent>(m_id);
	rigidbody->setVelocity(glm::vec3(0,bulletSpeed,0));
	_desiredVelocity = rigidbody->m_rigid->getLinearVelocity().length();
}

void Fracture::BulletScript::onUpdate(float dt)
{
	
	btVector3 currentVelocityDirection = rigidbody->m_rigid->getLinearVelocity();
	btScalar currentVelocty = currentVelocityDirection.length();
	if (currentVelocty < _desiredVelocity)
	{
		currentVelocityDirection *= _desiredVelocity / currentVelocty;
		rigidbody->m_rigid->setLinearVelocity(currentVelocityDirection);
	}
}

void Fracture::BulletScript::onLateUpdate(float dt)
{
}

void Fracture::BulletScript::onCollision(CollisionEvent* collision)
{
	FRACTURE_INFO("YEY I HIT ENEMY: {}",collision->entityB);
	destroy(collision->entityA);
	destroy(collision->entityB);
}
