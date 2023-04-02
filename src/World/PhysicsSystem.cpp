#include "FracturePCH.h"
#include "PhysicsSystem.h"
#include "World/Components.h"
#include "physx/PxPhysicsAPI.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsHelpers.h"

Fracture::PhysicsSystem::PhysicsSystem()
{
}

void Fracture::PhysicsSystem::AddForce(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& force, ForceMode mode)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to add force to non-dynamic PhysicsActor.");
		return;
	}
	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->addForce(PhysicsHelpers::ToPhysXVector(force), (physx::PxForceMode::Enum)mode);
}

void Fracture::PhysicsSystem::AddTorque(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& torque, ForceMode forceMode)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to add torque to non-dynamic PhysicsActor.");
		return;
	}

	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->addTorque(PhysicsHelpers::ToPhysXVector(torque), (physx::PxForceMode::Enum)forceMode);
}

void Fracture::PhysicsSystem::SetLinearVelocity(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& velocity)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to set velocity of non-dynamic PhysicsActor.");
		return;
	}

	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->setLinearVelocity(PhysicsHelpers::ToPhysXVector(velocity));
}

void Fracture::PhysicsSystem::SetAngularVelocity(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& velocity)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to set angular velocity of non-dynamic PhysicsActor.");
		return;
	}

	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->setAngularVelocity(PhysicsHelpers::ToPhysXVector(velocity));

}

void Fracture::PhysicsSystem::SetLinearDrag(const std::shared_ptr<RigidbodyComponent>& component, const float& drag)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to set linear drag of non-dynamic PhysicsActor.");
		return;
	}

	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->setLinearDamping(drag);
	component->LinearDrag = drag;
}

void Fracture::PhysicsSystem::SetAngularDrag(const std::shared_ptr<RigidbodyComponent>& component, const float& drag)
{
	if (!component->IsDynamic)
	{
		FRACTURE_WARN("Trying to set angular drag of non-dynamic PhysicsActor.");
	}

	physx::PxRigidDynamic* actor = PhysicsManager::GetRigidBody(component->GetID())->is<physx::PxRigidDynamic>();
	actor->setAngularDamping(drag);
	component->AngularDrag = drag;
}