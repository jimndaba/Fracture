#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H


#include "physx/PxPhysicsAPI.h"
#include "PhysicsEvents.h"

namespace Fracture
{
	class PhysicsScene;
	struct DestroyEntityEvent;

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
		PhysicsLayer() {};
		std::string Name = "";
		int LayerID;
		bool Changable = true;
		bool AlwaysFalse = false;
		bool AlwaysTrue = false;
		uint32_t Handle = 0;
	};

	struct PhysicsGroup
	{
		PhysicsGroup(Fracture::UUID id = UUID()) :GroupID(id) {}
		int GroupID;
		std::string Name;
		std::unordered_map<int, bool> InLayer;
		bool Changable = true;
		bool AlwaysFalse = false;
		bool AlwaysTrue = false;
	};

	struct PhsyicsSettings
	{
		glm::vec3 Gravity = glm::vec3(0.0f, -9.81, 0.0f);
		bool EnableCCD = true;
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
		void RemoveActor(UUID entity);
		void OnDebugDraw();

		void AddPhysicsGroup(int GroupID);
		void SetupFiltering(UUID entity);

		void OnAddActor(const std::shared_ptr<OnAddActorEvent>& evnt);
		void OnDestroyEntity(const std::shared_ptr<DestroyEntityEvent>& evnt);

		physx::PxPhysics& GetPhysicsSDK();
		physx::PxCpuDispatcher* GetCPUDispatcher();

		physx::PxRigidActor* GetRigidBody(const Fracture::UUID& entity);

		static PhysicsManager* Instance();

		std::map<int,std::shared_ptr<PhysicsLayer>> mPhysicsLayers;
		std::vector<int> mLayerOrder;

		std::map<int,std::shared_ptr<PhysicsGroup>> mPhysicsGroups;
		std::vector<int> mGroupOrder;
		std::vector<int> mAvialableGroupOrder;
		int NextGroupID = 2;

		PhsyicsSettings Settings;

	private:
		physx::PxPhysics* mPhysics;
		physx::PxCpuDispatcher* mDispacther;		
		physx::PxFoundation* gFoundation = NULL;
		physx::PxPvd* mPvd = NULL;
		physx::PxCooking* mCooking;

		static std::unique_ptr<PhysicsManager> mInstance;

		std::vector<UUID> mEntitiesToDelete;

		std::unordered_map<UUID, physx::PxRigidActor*> mActors;
		std::unordered_map<UUID, physx::PxShape*> mColliders;
		std::unordered_map<UUID, physx::PxMaterial*> mMaterials;
		std::unique_ptr<PhysicsScene> mScene;
		
	};



}

#endif