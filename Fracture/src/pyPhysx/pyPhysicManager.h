#pragma once
#ifndef PYPHSYICSMAN_H
#define PYPHSYICSMAN_H


#include "Physx/PxPhysicsAPI.h"




using namespace physx;
#define PVD_HOST "127.0.0.1"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

namespace Fracture
{

	class phPhysicsManager
	{

	public:
		phPhysicsManager();
		~phPhysicsManager();

		void init();

	private:
		PxDefaultErrorCallback m_ErrorCallback;
		PxDefaultAllocator	m_Allocator;
		PxFoundation* m_Foundation = NULL;
		PxPhysics* m_Physics = NULL;
		PxPvd* m_Pvd = NULL;
	};


}

#endif