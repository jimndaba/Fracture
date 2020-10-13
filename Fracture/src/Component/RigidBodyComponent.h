#pragma once
#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Component/Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include <memory>

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

		virtual void onStart();

	};

}

#endif