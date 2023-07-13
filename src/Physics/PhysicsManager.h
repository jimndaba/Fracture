#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H


#include "physx/PxPhysicsAPI.h"
#include "PhysicsEvents.h"

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
		
	enum class ForceMode : uint8_t
	{
		Force = 0,
		Impulse,
		VelocityChange,
		Acceleration
	};

	struct PhysicsLayer
	{
		std::string Name;
		std::unordered_map<Fracture::UUID, bool> InLayer;
	};

	struct PhysicsGroup
	{
		std::string Name;
		std::unordered_map<Fracture::UUID, bool> InGroup;
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

		void AddActor(UUID mEntity);
		void AddActors();

		void JointToParent(Fracture::UUID parent, Fracture::UUID child);

		bool HasActor(UUID entity);
		void RemoveActors();
		void OnDebugDraw();

		void OnAddActor(const std::shared_ptr<OnAddActorEvent>& evnt);

		physx::PxPhysics& GetPhysicsSDK();
		physx::PxCpuDispatcher* GetCPUDispatcher();

		physx::PxRigidActor* GetRigidBody(const Fracture::UUID& entity);

		static PhysicsManager* Instance();

		std::vector<PhysicsLayer> mPhysicsLayers;
		std::vector<PhysicsGroup> mPhysicsGroups;

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
		//std::unordered_map<UUID, physx::PxArticulation*> mArticulations;
		std::unique_ptr<PhysicsScene> mScene;
		
	};



}

#endif