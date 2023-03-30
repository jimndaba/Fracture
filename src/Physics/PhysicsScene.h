#pragma once
#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H

#include <PhysX/PxPhysicsAPI.h>

namespace Fracture
{

	class PhysicsScene
	{

	public:
		PhysicsScene(physx::PxPhysics* mPhysics, physx::PxCpuDispatcher* dispatcher);

		void FixedUpdate(float ts);

		static std::unique_ptr<PhysicsScene> Create(physx::PxPhysics* mPhysics, physx::PxCpuDispatcher* dispatcher);
		void Destroy();

		void OnDebugDraw();

		void AddActor(physx::PxRigidActor& actor);
		void RemoveActor(physx::PxRigidActor& actor);

	private:
		bool Advance(float ts);
		void SubstepStrategy(float ts, uint32_t& substepCount, float& substepSize);



	private:
		physx::PxScene* mScene;
		float mAccumulator = 0.0f;	
		float m_SubStepSize = 1.0f / 60.0f;
		float m_Accumulator = 0.0f;
		uint32_t m_NumSubSteps = 0;
		const uint32_t c_MaxSubSteps = 8;
	};



}

#endif