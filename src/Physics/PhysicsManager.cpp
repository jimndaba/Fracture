#include "FracturePCH.h"
#include "PhysicsManager.h"
#include "PhysicsScene.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "PhysicsHelpers.h"

using namespace physx;


std::unordered_map<Fracture::UUID, physx::PxRigidActor*> Fracture::PhysicsManager::mActors;
std::unordered_map<Fracture::UUID, physx::PxShape*> Fracture::PhysicsManager::mColliders;
std::unordered_map<Fracture::UUID, physx::PxMaterial*> Fracture::PhysicsManager::mMaterials;
physx::PxPhysics* Fracture::PhysicsManager::mPhysics;
physx::PxCpuDispatcher* Fracture::PhysicsManager::mDispacther;
std::unique_ptr<Fracture::PhysicsScene> Fracture::PhysicsManager::mScene;

Fracture::PhysicsManager::PhysicsManager()
{
}

void Fracture::PhysicsManager::Init()
{
	static PxDefaultErrorCallback errorCallback; // the error appears because of this line
	static PxDefaultAllocator allocatorCallback;

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);
	if (!gFoundation)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Foundation");
		return;
	}
		

	bool recordMemoryAllocations = true;

	mPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	PxTolerancesScale scale = PxTolerancesScale();
	//customizeTolerances(scale);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
		scale, recordMemoryAllocations, mPvd);
	if (!mPhysics)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Physics");
		return;
	}

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(scale));
	if (!mCooking)
	{
		FRACTURE_CRITICAL("Failed to Init Physx Cooking");
		return;
	}
	FRACTURE_INFO("Physics Engine Init");

	mDispacther = physx::PxDefaultCpuDispatcherCreate(2);
	
}

void Fracture::PhysicsManager::FixedUpdate(const float& dt)
{
	mScene->FixedUpdate(dt);

	
	const auto& components = SceneManager::GetAllComponents<RigidbodyComponent>();
	for (const auto& rigidbody : components)
	{
		const auto& transform = SceneManager::GetComponent<TransformComponent>(rigidbody->GetID());
		physx::PxTransform actorPose = mActors[rigidbody->GetID()]->getGlobalPose();
		transform->Position = PhysicsHelpers::FromPhysXVector(actorPose.p);		
		transform->Rotation = PhysicsHelpers::FromPhysXQuat(actorPose.q);
		transform->IsDirty = true;

	}
}

void Fracture::PhysicsManager::Shutdown()
{
	DestroyScene();

	mActors.clear();
	mColliders.clear();
	mMaterials.clear();
	mScene.release();
	
	if(mCooking)
		mCooking->release();
	if(mPhysics)
		mPhysics->release();
	if(gFoundation)
		gFoundation->release();
}

Fracture::PhysicsScene* Fracture::PhysicsManager::GetScene()
{
	return mScene.get();
}

void Fracture::PhysicsManager::CreateScene()
{
	mScene = PhysicsScene::Create(mPhysics, mDispacther);
}

void Fracture::PhysicsManager::DestroyScene()
{
	mActors.clear();
	mColliders.clear();
	mMaterials.clear();

	if(mScene)
		mScene->Destroy();
	//FRACTURE_INFO("Physics Scene Destroyed");
}

void Fracture::PhysicsManager::OnDebugDraw()
{
	if (mScene)
	{
		mScene->OnDebugDraw();
	}
}

void Fracture::PhysicsManager::AddActors(const UUID& mEntity)
{
	if (mScene)
	{
		if (SceneManager::HasComponent<RigidbodyComponent>(mEntity))
		{
			const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(mEntity);
			const auto& transform = SceneManager::GetComponent<TransformComponent>(mEntity);

			if (!rigidbody->IsDynamic)
			{
				mActors[mEntity] = mPhysics->createRigidStatic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
			}
			else
			{
				mActors[mEntity] = mPhysics->createRigidDynamic(PhysicsHelpers::ToPhysXTransform(transform->WorldTransform));
				FRACTURE_TRACE("Creating Dynamic Rigidbody");

				const auto& actor = mActors[mEntity]->is<physx::PxRigidDynamic>();
				actor->setMass(rigidbody->Mass);
				//actor->setSolverIterationCounts(settings.SolverIterations,settings.SolverVelocityIterations);
				actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, rigidbody->DetectionType == CollisionDetectionType::Continuous);
				actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, rigidbody->DetectionType == CollisionDetectionType::ContinuousSpeculative);
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
						mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mActors[mEntity], geometry, *mMaterials[mEntity]);
						mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));

						break;
					}
					case ColliderType::Box:
					{
						physx::PxBoxGeometry geometry = physx::PxBoxGeometry(collider->Size.x, collider->Size.y, collider->Size.z);
						mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mActors[mEntity], geometry, *mMaterials[mEntity]);
						mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						break;
					}
					case ColliderType::Capsule:
					{
						physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(collider->Radius, collider->Height);
						mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mActors[mEntity], geometry, *mMaterials[mEntity]);
						mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						break;
					}
					}

				}
				mActors[mEntity]->attachShape(*mColliders[mEntity]);
			}

			//const auto& name = SceneManager::CurrenScene()->GetComponent<TagComponent>(ID)->Name;
			//m_RigidActor->setName(name.c_str());
			mActors[mEntity]->userData = rigidbody.get();


			mScene->AddActor(*mActors[mEntity]);
		}
	}
}

void Fracture::PhysicsManager::RemoveActors(const UUID& mEntity)
{
	if (mScene)
		mScene->RemoveActor(*mActors[mEntity]);
}

physx::PxPhysics& Fracture::PhysicsManager::GetPhysicsSDK()
{
	return *mPhysics;
}

physx::PxCpuDispatcher* Fracture::PhysicsManager::GetCPUDispatcher()
{
	return mDispacther;
}

physx::PxFilterFlags Fracture::PhysicsManager::FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	/*
	if (filterData0.word2 == (uint32_t)RigidBodyComponent::CollisionDetectionType::Continuous || filterData1.word2 == (uint32_t)RigidBodyComponent::CollisionDetectionType::Continuous)
	{
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
	}
	*/

	if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		return physx::PxFilterFlag::eDEFAULT;
	}

	return physx::PxFilterFlag::eSUPPRESS; 

}
