#pragma once
#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Component/Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Physics/CollisionFilter.h"
#include <memory>
#include "glm/glm.hpp"
#include "Physics/PhysicsUtility.h"

namespace Fracture
{

	class RigidBodyComponent : public Component
	{

	public:

		RigidBodyComponent(uint32_t id,float _mass);
		~RigidBodyComponent();

		float Mass;


		btRigidBody* m_rigid;
		btTransform m_Transform;

		CollisionGroup collisionGroup;
		CollisionMask collisionMask;

		virtual void onStart();
		void setMass(float mass);
		void setVelocity(glm::vec3 velocity);
		void setPosition(glm::vec3 pos);
		void setRotation(glm::vec3 rot);
		void Translate(glm::vec3 position);

	};

}

#endif