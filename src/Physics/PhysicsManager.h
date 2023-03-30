#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "physx/PxPhysXConfig.h"
#include "physx/PxPhysicsAPI.h"
#include "physx/extensions/PxExtensionsAPI.h"
#include "physx/extensions/PxDefaultAllocator.h"
#include "physx/extensions/PxDefaultErrorCallback.h"
#include "physx/pvd/PxPvdTransport.h"

namespace Fracture
{
	class PhysicsScene;

	class PhysicsManager
	{

	public:
		PhysicsManager();

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

		physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);


	private:
		static physx::PxPhysics* mPhysics;
		static physx::PxCpuDispatcher* mDispacther;
		static std::unique_ptr<PhysicsScene> mScene;



		static std::unordered_map<UUID, physx::PxRigidActor*> mActors;
		static std::unordered_map<UUID, physx::PxShape*> mColliders;
		static std::unordered_map<UUID, physx::PxMaterial*> mMaterials;

		physx::PxFoundation* gFoundation = NULL;
		physx::PxPvd* mPvd;		
		physx::PxCooking* mCooking;
	};



}

#endif