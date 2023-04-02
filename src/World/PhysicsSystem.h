#pragma once
#ifndef PHSYCICSSYSTEM_H
#define PHSYCICSSYSTEM_H

namespace Fracture
{
	struct RigidbodyComponent;

	enum class ForceMode : uint8_t
	{
		Force = 0,
		Impulse,
		VelocityChange,
		Acceleration
	};


	struct PhysicsSystem
	{
		PhysicsSystem();

		static void AddForce (const std::shared_ptr<RigidbodyComponent>& component,const glm::vec3& force, ForceMode mode);
		static void AddTorque(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& torque, ForceMode forceMode);
		static void SetLinearVelocity(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& velocity);
		static void SetAngularVelocity(const std::shared_ptr<RigidbodyComponent>& component, const glm::vec3& velocity);
		static void SetLinearDrag(const std::shared_ptr<RigidbodyComponent>& component, const float& drag);
		static void SetAngularDrag(const std::shared_ptr<RigidbodyComponent>& component, const float& drag);
	};

}

#endif