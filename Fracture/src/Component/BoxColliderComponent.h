#pragma once
#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "Component/Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Physics/PhysicsUtility.h"

namespace Fracture
{


	class BoxColliderComponent: public Component
	{

	public:
		BoxColliderComponent(uint32_t id,float _x, float _y, float _z);
		~BoxColliderComponent() = default;

		BoxColliderComponent(const BoxColliderComponent& component, uint32_t id) :Component(id, ComponentType::BoxCollider)
		{
			m_boxCollider = component.m_boxCollider;
			m_Transform = component.m_Transform;
			X = component.X;
			Y = component.Y;
			Z = component.Z;
		}

		virtual void onStart();

		std::shared_ptr<btBoxShape> m_boxCollider;
		btTransform m_Transform;
		float X, Y, Z;

		void setScale(glm::vec3 sale);
		void Accept(ISceneProbe* visitor) override;
		std::shared_ptr<BoxColliderComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<BoxColliderComponent>(this->clone_impl(entityID));
		}


	private:

		virtual BoxColliderComponent* clone_impl(uint32_t entityID) const override
		{
			return new BoxColliderComponent(*this, entityID);
		}
	};


}

#endif