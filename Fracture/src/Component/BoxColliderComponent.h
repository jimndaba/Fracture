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
		BoxColliderComponent(UUID id,float _x, float _y, float _z);
		~BoxColliderComponent() = default;

		BoxColliderComponent(const BoxColliderComponent& component, UUID id) :Component(id)
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
		std::shared_ptr<BoxColliderComponent> clone(UUID id) const
		{
			return std::shared_ptr<BoxColliderComponent>(this->clone_impl(id));
		}

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

	private:

		virtual BoxColliderComponent* clone_impl(UUID id) const override
		{
			return new BoxColliderComponent(*this, id);
		}
	};


}

#endif