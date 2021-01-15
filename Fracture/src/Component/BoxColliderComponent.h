#pragma once
#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "Component/Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include <memory>
#include "Physics/PhysicsUtility.h"

namespace Fracture
{


	class BoxColliderComponent: public Component
	{

	public:
		BoxColliderComponent(uint32_t id,float _x, float _y, float _z);
		~BoxColliderComponent();

		virtual void onStart();

		btBoxShape* m_boxCollider;
		btTransform m_Transform;
		float X, Y, Z;

		void setScale(glm::vec3 sale);
		void Accept(ISceneProbe* visitor) override;
	};


}

#endif