#include "FracturePCH.h"
#include "LuaPhysicsBindings.h"
#include "Physics/PhysicsEvents.h"
#include "Physics/PhysicsHelpers.h"
#include "World/SceneManager.h"
#include "World/Components.h"

void LuaBindPhysics::BindPhysicsEvents(sol::state* lua)
{
	lua->new_usertype<Fracture::CollisionContext>("contactInfo",
		// Constructor 
		sol::constructors <
		Fracture::CollisionContext()
		>(),
		//Members
		"entity", &Fracture::CollisionContext::entity,
		"other", &Fracture::CollisionContext::other),
		//Metamethods
		sol::meta_function::to_string, [](Fracture::CollisionContext& e) { return fmt::format("contactInfo : Collsion:{},{}", e.entity, e.other);};

	std::initializer_list<std::pair<sol::string_view, Fracture::ForceMode>> ForceItems =
	{
		{"Acceleration", Fracture::ForceMode::Acceleration},
		{"Force", Fracture::ForceMode::Force},
		{"Impulse", Fracture::ForceMode::Impulse},
		{"VelocityChange", Fracture::ForceMode::VelocityChange},
			
	};
	lua->new_enum<Fracture::ForceMode, false>("forcemode", ForceItems);
}

void LuaBindPhysics::SetLinearVelocity(Fracture::UUID entity, glm::vec3 velocity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->setLinearVelocity(Fracture::PhysicsHelpers::ToPhysXVector(velocity));
	}

}

const glm::vec3& LuaBindPhysics::GetLinearVelocity(Fracture::UUID entity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to get velocity of non-dynamic PhysicsActor.");
			return glm::vec3(0.0f);
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		return Fracture::PhysicsHelpers::FromPhysXVector(actor->getLinearVelocity());
	}
	return glm::vec3(0.0f);
}

const glm::vec3& LuaBindPhysics::GetAngularVelocity(Fracture::UUID entity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to get angular velocity of non-dynamic PhysicsActor.");
			return glm::vec3(0.0f);
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		return Fracture::PhysicsHelpers::FromPhysXVector(actor->getAngularVelocity());
	}
	return glm::vec3(0.0f);
}

void LuaBindPhysics::SetAngularVelocity(Fracture::UUID entity, glm::vec3 velocity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set angular velocity of non-dynamic PhysicsActor.");
			return;
		}
		
		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->setAngularVelocity(Fracture::PhysicsHelpers::ToPhysXVector(velocity));
	}
}

void LuaBindPhysics::RotateRigidBody(Fracture::UUID entity, glm::vec3 value)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		physx::PxTransform transform = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->getGlobalPose();
		transform.q *= (physx::PxQuat(glm::radians(value.x), { 1.0f, 0.0f, 0.0f })
			* physx::PxQuat(glm::radians(value.y), { 0.0f, 1.0f, 0.0f })
			* physx::PxQuat(glm::radians(value.z), { 0.0f, 0.0f, 1.0f }));
		Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->setGlobalPose(transform);
	}
}

void LuaBindPhysics::Move(Fracture::UUID entity, glm::vec3 value)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::CharacterControllerComponent>(entity);
	const auto& cc = Fracture::PhysicsManager::Instance()->GetCharacterController(entity);
	if (cc && component)
	{	
		component->IsGrounded = false;
		physx::PxControllerFilters filters;
		physx::PxControllerCollisionFlags collisionFlags = cc->move(physx::PxVec3(value.x, value.y, value.z), component->MinMovementDist, 1.f / 60.f, filters);

		if (collisionFlags == physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			component->IsGrounded = true;
		}
	}
}

bool LuaBindPhysics::IsControllerGrounded(Fracture::UUID entity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::CharacterControllerComponent>(entity);
	if (component)
	{
		return component->IsGrounded;
	}

	return false;
}

void LuaBindPhysics::SetGravityDisabled(Fracture::UUID entity, bool value)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
	}
}

float LuaBindPhysics::GetMaxLinearVelocity(Fracture::UUID entity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to get max linear velocity of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		return actor->getMaxLinearVelocity();
	}
}

void LuaBindPhysics::SetMaxLinearVelocity(Fracture::UUID entity, float maxVelocity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set max linear velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->setMaxLinearVelocity(maxVelocity);
	}
}

float LuaBindPhysics::GetMaxAngularVelocity(Fracture::UUID entity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to get max angular velocity of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		return actor->getMaxAngularVelocity();
	}
	return 0.0f;
}

void LuaBindPhysics::SetMaxAngularVelocity(Fracture::UUID entity, float maxVelocity)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set max angular velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->setMaxAngularVelocity(maxVelocity);
		maxVelocity = maxVelocity;
	}
}

void LuaBindPhysics::AddForce(Fracture::UUID entity, glm::vec3 force, Fracture::ForceMode forceMode)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set max angular velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->addForce(Fracture::PhysicsHelpers::ToPhysXVector(force), (physx::PxForceMode::Enum)forceMode);
	}
}

void LuaBindPhysics::AddTorque(Fracture::UUID entity, glm::vec3 torque, Fracture::ForceMode forceMode)
{
	const auto& component = Fracture::SceneManager::GetComponent<Fracture::RigidbodyComponent>(entity);
	if (component)
	{
		if (!component->IsDynamic)
		{
			FRACTURE_WARN("Trying to set max angular velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = Fracture::PhysicsManager::Instance()->GetRigidBody(entity)->is<physx::PxRigidDynamic>();
		actor->addTorque(Fracture::PhysicsHelpers::ToPhysXVector(torque), (physx::PxForceMode::Enum)forceMode);
	}
}
