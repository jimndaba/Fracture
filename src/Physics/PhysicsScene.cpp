#include "FracturePCH.h"
#include "PhysicsScene.h"
#include "PhysicsManager.h"
#include "PhysicsHelpers.h"
#include "ContactListener.h"
#include "rendering/DebugRenderer.h"
#include "world/SceneManager.h"


static Fracture::ContactListener s_ContactListener;

Fracture::PhysicsScene::PhysicsScene(physx::PxPhysics* mPhysics, physx::PxCpuDispatcher* dispatcher)
{
    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.gravity = physx::PxVec3(0.0f,-9.81,0.0f);	
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;// (physx::PxSimulationFilterShader)PhysicsManager::FilterShader;
	sceneDesc.simulationEventCallback = &s_ContactListener;
	sceneDesc.frictionType = physx::PxFrictionType::eONE_DIRECTIONAL;	

	if (sceneDesc.isValid())
	{
		mScene = mPhysics->createScene(sceneDesc);
		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 2.0f);
		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
		
		physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		FRACTURE_INFO("Physics Scene Created!");
	}
	else
	{
		FRACTURE_ERROR("Scene Description invalid!");
	}
}

void Fracture::PhysicsScene::FixedUpdate(float ts)
{
	bool advanced = Advance(ts);	
	//physx::PxActorTypeFlags desiredTypes = physx::PxActorTypeFlag::eRIGID_DYNAMIC;// | physx::PxActorTypeFlag::eRIGID_STATIC;

	//FRACTURE_INFO("Dynamic: {}",(uint32_t)mScene->getNbActors(desiredTypes));
	
}

std::shared_ptr<Fracture::PhysicsScene> Fracture::PhysicsScene::Create(physx::PxPhysics* mPhysics, physx::PxCpuDispatcher* dispatcher)
{
	return std::make_shared<Fracture::PhysicsScene>(mPhysics,dispatcher);
}

void Fracture::PhysicsScene::Destroy()
{
	mScene->release();
	//mScene = nullptr;
}

void Fracture::PhysicsScene::OnDebugDraw()
{
	if (mScene)
	{
		const auto& rb = mScene->getRenderBuffer();
		for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
		{
			const physx::PxDebugLine& line = rb.getLines()[i];
			DebugRenderer::DrawLine(PhysicsHelpers::FromPhysXVector(line.pos0), PhysicsHelpers::FromPhysXVector(line.pos1));
		}
	}
}

void Fracture::PhysicsScene::AddActor(physx::PxRigidActor& actor)
{	
	mScene->addActor(actor);
}

void Fracture::PhysicsScene::RemoveActor(physx::PxRigidActor& actor)
{	
	mScene->removeActor(actor);
}

bool Fracture::PhysicsScene::Advance(float ts)
{
	SubstepStrategy(ts, m_NumSubSteps, m_SubStepSize);

	if (m_NumSubSteps == 0)
		return false;

	for (uint32_t i = 0; i < m_NumSubSteps; i++)
	{
		mScene->simulate(m_SubStepSize);
		mScene->fetchResults(true);
	}
	return true;
}

void Fracture::PhysicsScene::SubstepStrategy(float ts, uint32_t& substepCount, float& substepSize)
{
	if (m_Accumulator > m_SubStepSize)
	{
		m_Accumulator = 0.0f;
		
	}

	m_Accumulator += ts;
	if (m_Accumulator < m_SubStepSize)
	{
		substepCount = 0;
		return;
	}

	substepSize = m_SubStepSize;
	substepCount = glm::min(static_cast<uint32_t>(m_Accumulator / m_SubStepSize), c_MaxSubSteps);	
	m_Accumulator -= (float)substepCount * substepSize;
}
