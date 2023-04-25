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
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			FRACTURE_TRACE("On Contact Enter");
		}
	}
   
}

void Fracture::ContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for (physx::PxU32 i = 0; i < count; i++)
    {
        // ignore pairs when shapes have been deleted
        if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
            continue;

        FRACTURE_TRACE("On Trigger Enter");
    }
}

void Fracture::ContactListener::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
}
