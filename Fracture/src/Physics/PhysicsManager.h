#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Bullet/include/btBulletDynamicsCommon.h"

namespace Fracture
{


	class PhysicsManager
	{

	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();

		static void AddCollider(btCollisionShape* collider);
		static void RemoveCollider(btCollisionShape* collider);

		static void AddRigidBody(btRigidBody* body);
		static void RemoveRigidBody(btRigidBody* body);

		void onUpdate(float dt);

		static btAlignedObjectArray<btCollisionShape*> collisionShapes;

	private:
		static btDefaultCollisionConfiguration* collisionConfiguration;
		static btCollisionDispatcher* dispatcher;
		static btBroadphaseInterface* overlappingPairCache;
		static btSequentialImpulseConstraintSolver* solver;
		static btDiscreteDynamicsWorld* dynamicsWorld;

		
	};


}

#endif