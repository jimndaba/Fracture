#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H


#include "physx/PxPhysicsAPI.h"

namespace Fracture
{
	class PhysicsScene;

	struct FilterGroup
	{
		enum Enum
		{
			eOne = (1 << 0),
			eTwo = (1 << 1),
			eThree = (1 << 2),
			eFour = (1 << 3),
			eFive = (1 << 4),
		};
	};

	class PhysicsManager 
	{

	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();
		void FixedUpdate(const float& dt);
		void Shutdown();

		PhysicsScene* GetScene();
		void CreateScene();
		void DestroyScene();

		void AddActors(const UUID& mEntity);
		void RemoveActors(const UUID& mEntity);
		void OnDebugDraw();

		physx::PxPhysics& GetPhysicsSDK();
		physx::PxCpuDispatcher* GetCPUDispatcher();

		physx::PxRigidActor* GetRigidBody(const Fracture::UUID& entity);

		static PhysicsManager* Instance();

	private:
		physx::PxPhysics* mPhysics;
		physx::PxCpuDispatcher* mDispacther;		
		physx::PxFoundation* gFoundation = NULL;
		physx::PxPvd* mPvd = NULL;
		physx::PxCooking* mCooking;

		static std::unique_ptr<PhysicsManager> mInstance;


		std::unordered_map<UUID, physx::PxRigidActor*> mActors;
		std::unordered_map<UUID, physx::PxShape*> mColliders;
		std::unordered_map<UUID, physx::PxMaterial*> mMaterials;
		std::unique_ptr<PhysicsScene> mScene;
		
	};



}

#endif