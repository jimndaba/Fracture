#pragma once
#ifndef ENEMYSCRIPT_H
#define ENEMYSCRIPT_H

#include "Fracture.h"

namespace Fracture
{

	class EnemyScript : public GameLogic
	{
	public:
		EnemyScript(uint32_t id);

		virtual void onStart();


		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		void onCollision(CollisionEvent* collision);


		float smoothTime = 0.3F;
		float EnemySpeed = 4.0f;
	private:
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<RigidBodyComponent> rigidbody;
		uint32_t m_id;
		btScalar   _desiredVelocity;
		glm::vec3 position;

	};
}

#endif