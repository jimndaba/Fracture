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

		RigidBodyComponent(UUID id,const float& _mass);
		~RigidBodyComponent() = default;

		RigidBodyComponent(const RigidBodyComponent& component, UUID id) :Component(id)
		{
			m_rigid = component.m_rigid;
			Mass = component.Mass;
			m_Transform = component.m_Transform;
			myMotionState = component.myMotionState;
			boxCollider = component.boxCollider;

			collisionGroup = component.collisionGroup;
			collisionMask = component.collisionMask;
		}


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
		
		std::shared_ptr<RigidBodyComponent> clone(UUID id) const
		{
			return std::shared_ptr<RigidBodyComponent>(this->clone_impl(id));
		}

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

	private:

		virtual RigidBodyComponent* clone_impl(UUID id) const override
		{
			return new RigidBodyComponent(*this, id);
		}
	};

}

#endif