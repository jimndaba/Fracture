#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "bullet/btBulletDynamicsCommon.h"


namespace Fracture
{
	struct PhysicsConfiguration
	{
		glm::vec3 Gravity = { 0, -10, 0 };

	};

	class PhysicsManager
	{

	public:
		PhysicsManager(const PhysicsConfiguration& config);

		void OnInit();
		void OnShutdown();

		void OnClearPhysicsWorld();

		void OnPhysicsUpdate(float dt);
		void OnCollisionUpdate();

		void SetGravity(const glm::vec3& gravity);
		void SetMovementConstraints(const Fracture::UUID& entity, const bool c[3]);
		void SetRotationConstraints(const Fracture::UUID& entity, const bool c[3]);

		void AddRigidBodyToWorld(Fracture::UUID entity);
		void AttachCollisionShape(const Fracture::UUID& entity);

		void RemoveCollider(const Fracture::UUID& entity);
		void RemoveRigidBody(const Fracture::UUID& entity);

		PhysicsConfiguration Config;
		std::unordered_map<Fracture::UUID, std::unique_ptr<btRigidBody>> mRigidBodies;
		std::unordered_map<Fracture::UUID, std::unique_ptr<btCollisionShape>> mCollisionShapes;

	private:
		btBroadphaseInterface* _broadphase;
		btDefaultCollisionConfiguration* _collisionConfig;
		btCollisionDispatcher* _collisionDispatcher;
		btSequentialImpulseConstraintSolver* _solver;
		btDiscreteDynamicsWorld* _world;

	};




}

#endif