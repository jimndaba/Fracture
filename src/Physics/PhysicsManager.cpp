#include "FracturePCH.h"
#include "PhysicsManager.h"
#include "PhysicsScene.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "PhysicsHelpers.h"
#include "World/TransformSystem.h"
#include "EventSystem/Eventbus.h"
#include "World/WorldEvents.h"

using namespace physx;

std::unique_ptr<Fracture::PhysicsManager> Fracture::PhysicsManager::mInstance;

static PxDefaultErrorCallback errorCallback;
static PxDefaultAllocator allocatorCallback;

Fracture::PhysicsManager::PhysicsManager()
{
}

Fracture::PhysicsManager::~PhysicsManager()
{
	if(mInstance)
		mInstance.release();
}

void Fracture::PhysicsManager::Init()
{
	//mInstance = std::make_unique<PhysicsManager>();

	

	mInstance->gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);
	if (!mInstance->gFoundation)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Foundation");
		return;
	}
		

	bool recordMemoryAllocations = true;

	mInstance->mPvd = PxCreatePvd(*mInstance->gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mInstance->mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	mInstance->mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mInstance->gFoundation,
		PxTolerancesScale(), recordMemoryAllocations, mInstance->mPvd);
	if (!mInstance->mPhysics)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Physics");
		return;
	}

	mInstance->mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mInstance->gFoundation, PxCookingParams(PxTolerancesScale()));

	if (!mInstance->mCooking)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Cooking");
		return;
	}
	FRACTURE_INFO("Physics Engine Init");

	int Default_layer = 0;
	mPhysicsLayers[Default_layer] = std::make_shared<PhysicsLayer>();
	mPhysicsLayers[Default_layer]->LayerID = Default_layer;
	mPhysicsLayers[Default_layer]->Name = "Default";
	mPhysicsLayers[Default_layer]->Changable = false;
	mPhysicsLayers[Default_layer]->AlwaysTrue = true;
	mPhysicsLayers[Default_layer]->Handle = (1 << 0);
	mLayerOrder.push_back(Default_layer);

	int layer_id = 1;
	for (uint32_t i = 1; i < 32; i++)
	{
		auto layer = std::make_shared<PhysicsLayer>();
		layer->LayerID = layer_id;
		mPhysicsLayers[layer->LayerID] = layer;
		mPhysicsLayers[layer->LayerID]->Handle = (1 << i);
		mLayerOrder.push_back(layer->LayerID);
		layer_id += 1;
	}
	

	{
		auto group_all = std::make_shared<PhysicsGroup>();
		group_all->GroupID = 1;
		group_all->InLayer[Default_layer] = true;
		group_all->Name = "All";
		group_all->Changable = false;
		group_all->AlwaysTrue = true;
		mPhysicsGroups[group_all->GroupID] = group_all;
		mGroupOrder.push_back(group_all->GroupID);
	}
	{
		auto group_None = std::make_shared<PhysicsGroup>();
		group_None->GroupID = 0;
		group_None->InLayer[Default_layer] = false;
		group_None->Name = "None";
		group_None->Changable = false;
		group_None->AlwaysFalse = true;
		mPhysicsGroups[group_None->GroupID] = group_None;
		mGroupOrder.push_back(group_None->GroupID);
	}
	mInstance->mDispacther = physx::PxDefaultCpuDispatcherCreate(2);
	Eventbus::Subscribe(this, &Fracture::PhysicsManager::OnAddActor);
	Eventbus::Subscribe(this, &Fracture::PhysicsManager::OnDestroyEntity);
	
	
}

void Fracture::PhysicsManager::FixedUpdate(const float& dt)
{
	mInstance->mScene->FixedUpdate(dt);
	
	const auto& components = SceneManager::GetAllComponents<RigidbodyComponent>();
	for (const auto& rigidbody : components)
	{
		if (!HasActor(rigidbody->GetID()))
			continue;

		const auto& transform = SceneManager::GetComponent<TransformComponent>(rigidbody->GetID());
		physx::PxTransform actorPose = mInstance->mActors[rigidbody->GetID()]->getGlobalPose();
		transform->Position = PhysicsHelpers::FromPhysXVector(actorPose.p);		
		transform->Rotation = PhysicsHelpers::FromPhysXQuat(actorPose.q);
		transform->IsDirty = true;
	}

	
	for (const auto& entity : mEntitiesToDelete)
	{
		RemoveActor(entity);
	}
	mEntitiesToDelete.clear();
}


void Fracture::PhysicsManager::Shutdown()
{
	if (mInstance)
	{
		mInstance->DestroyScene();

		if (mInstance->mPhysics)
		{
			mInstance->mPhysics->release();
			mInstance->mPhysics = NULL;
		}

		if (mInstance->mCooking)
		{
			mInstance->mCooking->release();
			mInstance->mCooking = NULL;
		}

		if (mInstance->mPvd)
		{
			mInstance->mPvd->release();
			mInstance->mPvd = NULL;
		}

		if (gFoundation)
		{
			mInstance->gFoundation->release();
			mInstance->gFoundation = NULL;
		}
		mInstance.release();
	}

}

Fracture::PhysicsScene* Fracture::PhysicsManager::GetScene()
{
	return mInstance->mScene.get();
}

void Fracture::PhysicsManager::CreateScene()
{
	mInstance->mScene = PhysicsScene::Create(mInstance->Settings ,mInstance->mPhysics, mInstance->mDispacther);
}

void Fracture::PhysicsManager::DestroyScene()
{
	if (mInstance->mScene)
	{
		mInstance->mActors.clear();
		mInstance->mColliders.clear();
		mInstance->mMaterials.clear();

		mInstance->mScene->Destroy();
		mInstance->mScene.reset();
			
		FRACTURE_INFO("Physics Scene Destroyed");
	}
}

void Fracture::PhysicsManager::OnDebugDraw()
{
	if (mInstance->mScene)
	{
		mInstance->mScene->OnDebugDraw();
	}
}

void Fracture::PhysicsManager::AddPhysicsGroup(int GroupID)
{
	mInstance->mPhysicsGroups[GroupID] = std::make_shared<Fracture::PhysicsGroup>(GroupID);
	mInstance->mGroupOrder.push_back(GroupID);
}

void Fracture::PhysicsManager::SetupFiltering(UUID entity)
{
	if (SceneManager::HasComponent<ColliderComponent>(entity))
	{
		PxFilterData filterData;
		const auto& collider = SceneManager::GetComponent<ColliderComponent>(entity);
		if (collider)
		{
			//Which Layer this belongs to;
			filterData.word0 = (physx::PxU32)(1 << mPhysicsLayers[collider->CollisionLayer]->LayerID);	
			
			physx::PxU32 mask = 0;
			for (auto s : mPhysicsGroups[collider->CollisionGroup]->InLayer)
			{
				if(s.second)
					mask |= (1 << mPhysicsLayers[s.first]->LayerID);
			}
			filterData.word1 = mask;			
		}

		mInstance->mColliders[entity]->setSimulationFilterData(filterData);
	}


}

void Fracture::PhysicsManager::OnAddActor(const std::shared_ptr<OnAddActorEvent>& evnt)
{
	if (HasActor(evnt->id))
		return;

	if (SceneManager::HasComponent<RigidbodyComponent>(evnt->id))
	{
		TransformSystem::UpdatePrefabs(evnt->id);
		AddActor(evnt->id);
	}

}

void Fracture::PhysicsManager::OnDestroyEntity(const std::shared_ptr<DestroyEntityEvent>& evnt)
{
	mEntitiesToDelete.push_back(evnt->ID);
}

void Fracture::PhysicsManager::AddActor(UUID mEntity)
{
	
	if (mInstance->mScene)
	{
		const auto& transform = SceneManager::GetComponent<TransformComponent>(mEntity);
		const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(mEntity);
		if (!rigidbody->IsDynamic)
		{
			mInstance->mActors[mEntity] = mInstance->mPhysics->createRigidStatic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
		}
		else
		{
			mInstance->mActors[mEntity] = mInstance->mPhysics->createRigidDynamic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
			FRACTURE_TRACE("Creating Dynamic Rigidbody");

			const auto& actor = mInstance->mActors[mEntity]->is<physx::PxRigidDynamic>();
			actor->setMass(rigidbody->Mass);
		
			//actor->setSolverIterationCounts(settings.SolverIterations,settings.SolverVelocityIterations);
			actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, rigidbody->DetectionType == CollisionDetectionType::Continuous);
			actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, rigidbody->DetectionType == CollisionDetectionType::ContinuousSpeculative);
		

			physx::PxRigidDynamicLockFlags flags; 			
			if (rigidbody->AngularConstraints[0])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
			}

			if (rigidbody->AngularConstraints[1])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
			}

			if (rigidbody->AngularConstraints[2])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
			}

			if (rigidbody->LinearConstraints[0])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
			}

			if (rigidbody->LinearConstraints[1])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
			}

			if (rigidbody->LinearConstraints[2])
			{
				flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
			}
			actor->setRigidDynamicLockFlags(flags);

			PxRigidBodyExt::updateMassAndInertia(*actor, 10.0f);


		}


		if (SceneManager::HasComponent<ColliderComponent>(mEntity))
		{
			const auto& collider = SceneManager::GetComponent<ColliderComponent>(mEntity);
			if (collider)
			{
				switch (collider->Shape)
				{
				case ColliderType::Sphere:
				{
					physx::PxSphereGeometry geometry = physx::PxSphereGeometry(collider->Radius);
					mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
					mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
					mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
					SetupFiltering(mEntity);

					break;
				}
				case ColliderType::Box:
				{
					physx::PxBoxGeometry geometry = physx::PxBoxGeometry(collider->Size.x, collider->Size.y, collider->Size.z);
					mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
					mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
					mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
					SetupFiltering(mEntity);
					break;
				}
				case ColliderType::Capsule:
				{
					physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(collider->Radius, collider->Height);
					mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
					mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
					mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
					SetupFiltering(mEntity);

					break;
				}
				}

			}
			mInstance->mActors[mEntity]->attachShape(*mInstance->mColliders[mEntity]);
			mInstance->mColliders[mEntity]->release();
			mInstance->mColliders.erase(mEntity);
		}

		//const auto& name = SceneManager::CurrenScene()->GetComponent<TagComponent>(ID)->Name;
		//m_RigidActor->setName(name.c_str());
		mInstance->mActors[mEntity]->userData = rigidbody.get();
		mInstance->mScene->AddActor(*mInstance->mActors[mEntity]);
	}
}

void Fracture::PhysicsManager::AddActors()
{
	if (mInstance->mScene)
	{
		for (const auto& rigidbody : SceneManager::GetAllComponents<RigidbodyComponent>())
		{
			auto mEntity = rigidbody->GetID();
			const auto& transform = SceneManager::GetComponent<TransformComponent>(mEntity);
			

			if (!rigidbody->IsDynamic)
			{
				mInstance->mActors[mEntity] = mInstance->mPhysics->createRigidStatic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
			}
			else
			{
				mInstance->mActors[mEntity] = mInstance->mPhysics->createRigidDynamic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
				FRACTURE_TRACE("Creating Dynamic Rigidbody");

				const auto& actor = mInstance->mActors[mEntity]->is<physx::PxRigidDynamic>();
				actor->setMass(rigidbody->Mass);
				//actor->setSolverIterationCounts(settings.SolverIterations,settings.SolverVelocityIterations);
				actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, rigidbody->DetectionType == CollisionDetectionType::Continuous);
				actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, rigidbody->DetectionType == CollisionDetectionType::ContinuousSpeculative);

				if(rigidbody->IsKinematic)
					actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);

				PxRigidBodyExt::updateMassAndInertia(*actor, 10.0f);

				physx::PxRigidDynamicLockFlags flags;
				if (rigidbody->AngularConstraints[0])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
				}

				if (rigidbody->AngularConstraints[1])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
				}

				if (rigidbody->AngularConstraints[2])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
				}

				if (rigidbody->LinearConstraints[0])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
				}

				if (rigidbody->LinearConstraints[1])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
				}

				if (rigidbody->LinearConstraints[2])
				{
					flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
				}
				actor->setRigidDynamicLockFlags(flags);				
			}

			if (SceneManager::HasComponent<ColliderComponent>(mEntity))
			{
				const auto& collider = SceneManager::GetComponent<ColliderComponent>(mEntity);
				if (collider)
				{
					switch (collider->Shape)
					{
					case ColliderType::Sphere:
					{
						physx::PxSphereGeometry geometry = physx::PxSphereGeometry(collider->Radius);
						mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
						mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						SetupFiltering(mEntity);
						break;
					}
					case ColliderType::Box:
					{
						physx::PxBoxGeometry geometry = physx::PxBoxGeometry(collider->Size.x, collider->Size.y, collider->Size.z);
						mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
						mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						SetupFiltering(mEntity);					
						break;
					}
					case ColliderType::Capsule:
					{
						physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(collider->Radius, collider->Height);
						mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
						mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						SetupFiltering(mEntity);
						break;
					}
					}

				}
				mInstance->mActors[mEntity]->attachShape(*mInstance->mColliders[mEntity]);
				mInstance->mColliders[mEntity]->release();
			}

			//const auto& name = SceneManager::CurrenScene()->GetComponent<TagComponent>(ID)->Name;
			//m_RigidActor->setName(name.c_str());
			mInstance->mActors[mEntity]->userData = rigidbody.get();
			mInstance->mScene->AddActor(*mInstance->mActors[mEntity]);
		}	
	}
}

void Fracture::PhysicsManager::JointToParent(Fracture::UUID parent, Fracture::UUID child)
{
	PxRevoluteJointCreate(GetPhysicsSDK()
		, mActors[parent],mActors[parent]->getGlobalPose(),
		mActors[child], mActors[child]->getGlobalPose());
}

bool Fracture::PhysicsManager::HasActor(UUID entity)
{
	return (mInstance->mActors.find(entity) != mInstance->mActors.end());	
}

void Fracture::PhysicsManager::RemoveActors()
{
	for (const auto& actor : SceneManager::GetAllComponents<RigidbodyComponent>())
	{
		if (mInstance->mScene)
			mInstance->mScene->RemoveActor(*mInstance->mActors[actor->GetID()]);
	}

}

void Fracture::PhysicsManager::RemoveActor(UUID entity)
{
	if (HasActor(entity))
	{
		mInstance->mScene->RemoveActor(*mInstance->mActors[entity]);
		mInstance->mActors[entity]->release();
		mInstance->mActors.erase(entity);
	}
}

physx::PxPhysics& Fracture::PhysicsManager::GetPhysicsSDK()
{
	return *mInstance->mPhysics;
}

physx::PxCpuDispatcher* Fracture::PhysicsManager::GetCPUDispatcher()
{
	return mInstance->mDispacther;
}



physx::PxRigidActor* Fracture::PhysicsManager::GetRigidBody(const Fracture::UUID& entity)
{
	if (mInstance->mActors.find(entity) == mInstance->mActors.end())
		return nullptr;

	return mInstance->mActors[entity];
}

Fracture::PhysicsManager* Fracture::PhysicsManager::Instance()
{
	if (!mInstance)
	{
		mInstance = std::make_unique<PhysicsManager>();
	}
	return mInstance.get();
}