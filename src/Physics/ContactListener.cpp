#include "FracturePCH.h"
#include "ContactListener.h"

void Fracture::ContactListener::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void Fracture::ContactListener::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}

void Fracture::ContactListener::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void Fracture::ContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
}

void Fracture::ContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
}

void Fracture::ContactListener::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
}
