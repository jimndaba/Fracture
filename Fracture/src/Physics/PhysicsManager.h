#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Bullet/include/btBulletDynamicsCommon.h"



#include "CollisionFilter.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>


namespace Fracture
{
	struct RayHit;
	class Ray; 
	class PhysicsDebugDraw;

	class PhysicsManager
	{

	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();

		void setGravity(float x,float y, float z);

		static void ClearScene();

		static void AddCollider(int id,btCollisionShape* collider);
		static void RemoveCollider(btCollisionShape* collider);

		static void AddRigidBody(int id,btRigidBody* body);
		static void AddRigidBody(int id, btRigidBody* body, Fracture::CollisionGroup group, Fracture::CollisionMask mask);
		static void RemoveRigidBody(btRigidBody* body);

		static bool RayCast(Ray rayIn, RayHit& out);

		void startPhysics();

		void onUpdate(float dt);

		static void DrawDebug();

		void checkCollision();


		static btAlignedObjectArray<btCollisionShape*> collisionShapes;

		

	private:
		static btDefaultCollisionConfiguration* collisionConfiguration;
		static btCollisionDispatcher* dispatcher;
		static btBroadphaseInterface* overlappingPairCache;
		static btSequentialImpulseConstraintSolver* solver;
		static btDiscreteDynamicsWorld* dynamicsWorld;
		PhysicsDebugDraw* m_debug;

		btVector3 m_gravity;

		static std::vector<int> rigid_ids;
		static std::vector<int> collision_ids;
	};


}

#endif