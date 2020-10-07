#pragma once
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
#include "Physx/include/PxPhysicsAPI.h"
#include <Physx\include\extensions\PxDefaultAllocator.h>



namespace Fracture
{
	using namespace physx;

	class PhysicsManager
	{

	public:
		PhysicsManager();
		~PhysicsManager();

	private:

		PxDefaultErrorCallback gDefaultErrorCallback;
		PxDefaultAllocator gDefaultAllocatorCallbac;


	};


}

#endif