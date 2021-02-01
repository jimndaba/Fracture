#pragma once
#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Component/Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Physics/CollisionFilter.h"
#include "Physics/PhysicsUtility.h"

namespace Fracture
{

	class RigidBodyComponent : public Component
	{

	public:

		RigidBodyComponent(uint32_t id,const float& _mass);
		~RigidBodyComponent() = default;

		float Mass;
		std::shared_ptr<btRigidBody> m_rigid;
		std::shared_ptr<btTransform> m_Transform;
		std::shared_ptr<btDefaultMotionState> myMotionState;
		std::shared_ptr<btCollisionShape> boxCollider;

		CollisionGroup collisionGroup;
		CollisionMask collisionMask;

		virtual void onStart();
		void setMass(const float& mass);
		void setVelocity(const glm::vec3& velocity);
		void setPosition(const glm::vec3& pos);
		void setRotation(const glm::vec3& rot);
		void Translate(const glm::vec3& position);
		void Accept(ISceneProbe* visitor) override;
	};

}

#endif