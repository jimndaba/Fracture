#pragma once
#ifndef BULLETSCRIPT_H
#define BULLETSCRIPT_H

#include "Fracture.h"


namespace Fracture
{

	class BulletScript :public GameLogic
	{

	public:
		BulletScript(uint32_t id, glm::vec3 position);
		~BulletScript();


		virtual void onStart();

		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		void onCollision(CollisionEvent* collision);

		std::shared_ptr<RigidBodyComponent> rigidbody;

		uint32_t m_id;
		float bulletSpeed = 12.0f;
		btScalar   _desiredVelocity;
		glm::vec3 m_position = glm::vec3();
	};


}


#endif