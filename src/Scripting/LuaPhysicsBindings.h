#pragma once
#ifndef LUABINDPHYSICS_H
#define LUABINDPHYSICS_H

#include "sol/sol.hpp"
#include "Physics/PhysicsManager.h"

namespace LuaBindPhysics
{
	void BindPhysicsEvents(sol::state* lua);

	void SetLinearVelocity(Fracture::UUID entity, glm::vec3 velocity);
	const glm::vec3& GetLinearVelocity(Fracture::UUID entity);

	const glm::vec3& GetAngularVelocity(Fracture::UUID entity);
	void SetAngularVelocity(Fracture::UUID entity, glm::vec3 velocity);

	void RotateRigidBody(Fracture::UUID entity, glm::vec3 value);

	void Move(Fracture::UUID entity, glm::vec3 value);
	bool IsControllerGrounded(Fracture::UUID entity);

	void SetGravityDisabled(Fracture::UUID entity, bool value);

	float GetMaxLinearVelocity(Fracture::UUID entity);
	void SetMaxLinearVelocity(Fracture::UUID entity, float maxVelocity);
	float GetMaxAngularVelocity(Fracture::UUID entity) ;
	void SetMaxAngularVelocity(Fracture::UUID entity, float maxVelocity);
	
	void AddForce(Fracture::UUID entity, glm::vec3 force, Fracture::ForceMode forceMode);
	void AddTorque(Fracture::UUID entity, glm::vec3 torque, Fracture::ForceMode forceMode);

}

#endif