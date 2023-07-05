#include "FracturePCH.h"
#include "PhysicsManager.h"
#include "PhysicsScene.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "PhysicsHelpers.h"
#include "World/TransformSystem.h"
#include "EventSystem/Eventbus.h"
using namespace physx;

std::unique_ptr<Fracture::PhysicsManager> Fracture::PhysicsManager::mInstance;

//std::unordered_map<Fracture::UUID, physx::PxRigidActor*> Fracture::PhysicsManager::mActors;
//std::unordered_map<Fracture::UUID, physx::PxShape*> Fracture::PhysicsManager::mColliders;
//std::unordered_map<Fracture::UUID, physx::PxMaterial*> Fracture::PhysicsManager::mMaterials;
//physx::PxPhysics* Fracture::PhysicsManager::mPhysics;
//physx::PxCpuDispatcher* Fracture::PhysicsManager::mDispacther;
//std::unique_ptr<Fracture::PhysicsScene> Fracture::PhysicsManager::mScene;

static PxDefaultErrorCallback errorCallback;
static PxDefaultAllocator allocatorCallback;

Fracture::PhysicsManager::PhysicsManager()
{
}

Fracture::PhysicsManager::~PhysicsManager()
{

}

void Fracture::PhysicsManager::Init()
{
	mInstance = std::make_unique<PhysicsManager>();

	

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

	mInstance->mDispacther = physx::PxDefaultCpuDispatcherCreate(2);
	Eventbus::Subscribe(this, &Fracture::PhysicsManager::OnAddActor);
	
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
	}

}

Fracture::PhysicsScene* Fracture::PhysicsManager::GetScene()
{
	return mInstance->mScene.get();
}

void Fracture::PhysicsManager::CreateScene()
{
	mInstance->mScene = PhysicsScene::Create(mInstance->mPhysics, mInstance->mDispacther);
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

void Fracture::PhysicsManager::OnAddActor(const std::shared_ptr<OnAddActorEvent>& evnt)
{
	if (HasActor(evnt->id))
		return;

	if (SceneManager::HasComponent<RigidbodyComponent>(evnt->id))
	{
		TransformSystem::UpdatePrefabs(evnt->manager, evnt->id);
		AddActor(evnt->id);
	}

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
					PxFilterData filterData;
					filterData.word0 = FilterGroup::eOne; // word0 = own ID
					filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
					mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);

					break;
				}
				case ColliderType::Box:
				{
					physx::PxBoxGeometry geometry = physx::PxBoxGeometry(collider->Size.x, collider->Size.y, collider->Size.z);
					mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
					mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
					mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));

					PxFilterData filterData;
					filterData.word0 = FilterGroup::eOne; // word0 = own ID
					filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
					mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);
					break;
				}
				case ColliderType::Capsule:
				{
					physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(collider->Radius, collider->Height);
					mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
					mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
					mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
					PxFilterData filterData;
					filterData.word0 = FilterGroup::eOne; // word0 = own ID
					filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
					mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);

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
						PxFilterData filterData;
						filterData.word0 = FilterGroup::eOne; // word0 = own ID
						filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
						mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);

						break;
					}
					case ColliderType::Box:
					{
						physx::PxBoxGeometry geometry = physx::PxBoxGeometry(collider->Size.x, collider->Size.y, collider->Size.z);
						mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
						mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));

						PxFilterData filterData;
						filterData.word0 = FilterGroup::eOne; // word0 = own ID
						filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
						mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);
						break;
					}
					case ColliderType::Capsule:
					{
						physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(collider->Radius, collider->Height);
						mInstance->mMaterials[mEntity] = PhysicsManager::GetPhysicsSDK().createMaterial(0.6, rigidbody->Friction, rigidbody->Bouncyness);
						mInstance->mColliders[mEntity] = physx::PxRigidActorExt::createExclusiveShape(*mInstance->mActors[mEntity], geometry, *mInstance->mMaterials[mEntity]);
						mInstance->mColliders[mEntity]->setLocalPose(PhysicsHelpers::ToPhysXTransform(collider->Offset, glm::vec3(0.0f)));
						PxFilterData filterData;
						filterData.word0 = FilterGroup::eOne; // word0 = own ID
						filterData.word1 = FilterGroup::eOne;	// word1 = ID mask to filter pairs that trigger a contact callback
						mInstance->mColliders[mEntity]->setSimulationFilterData(filterData);

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
	return mInstance.get();
}