#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Bullet/include/btBulletDynamicsCommon.h"
#include "CollisionFilter.h"
#include <map>
#include <vector>

namespace Fracture
{


	class PhysicsManager
	{

	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();

		void setGravity(float x,float y, float z);

		static void AddCollider(int id,btCollisionShape* collider);
		static void RemoveCollider(btCollisionShape* collider);

		static void AddRigidBody(int id,btRigidBody* body);
		static void AddRigidBody(int id, btRigidBody* body, Fracture::CollisionGroup group, Fracture::CollisionMask mask);
		static void RemoveRigidBody(btRigidBody* body);

		void startPhysics();

		void onUpdate(float dt);

		void checkCollision();

		static btAlignedObjectArray<btCollisionShape*> collisionShapes;

		

	private:
		static btDefaultCollisionConfiguration* collisionConfiguration;
		static btCollisionDispatcher* dispatcher;
		static btBroadphaseInterface* overlappingPairCache;
		static btSequentialImpulseConstraintSolver* solver;
		static btDiscreteDynamicsWorld* dynamicsWorld;

		btVector3 m_gravity;

		static std::vector<int> rigid_ids;
		static std::vector<int> collision_ids;
	};


}

#endif