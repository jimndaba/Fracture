#include "pyPhysicManager.h"

Fracture::phPhysicsManager::phPhysicsManager()
{

}

Fracture::phPhysicsManager::~phPhysicsManager()
{
	PX_RELEASE(m_Physics);

	if (m_Pvd)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_Foundation);
}

void Fracture::phPhysicsManager::init()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
}
