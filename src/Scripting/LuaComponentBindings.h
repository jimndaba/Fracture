
#pragma once
#ifndef LUABINDCOMPS_H
#define LUABINDCOMPS_H

#include "sol/sol.hpp"

namespace LuaBindComponents
{
	void BindTagComponent(sol::state& lua);
	void BindTransformComponent(sol::state& lua);
	void BindHierachyComponent(sol::state& lua);
	void BindMeshComponent(sol::state& lua);
	void BindColliderComponent(sol::state& lua);
	void BindRigidbodyComponent(sol::state& lua);
	void BindPointLightComponent(sol::state& lua);
	void BindSpotLightComponent(sol::state& lua);
	void BindSunLightComponent(sol::state& lua);
	void BindCameraComponent(sol::state& lua);
	void BindAnimatorComponent(sol::state& lua);

	glm::vec3 GetPosition(Fracture::UUID entity);
	glm::vec3 GetScale(Fracture::UUID entity);
	glm::vec3 GetRotation(Fracture::UUID entity);

	void SetPosition(Fracture::UUID entity, glm::vec3 value);
	void SetRotation(Fracture::UUID entity, glm::vec3 value);
	void SetScale(Fracture::UUID entity, glm::vec3 value);


	void Translate(Fracture::UUID entity, glm::vec3 value);
	void Rotate(Fracture::UUID entity, glm::vec3 value);
	void LookAt(Fracture::UUID entity, glm::vec3 value, glm::vec3 up);

	void Instantiate(Fracture::UUID entity, glm::vec3 position, glm::vec3 rotation = glm::vec3(0));

	glm::vec3 Vec3Lerp(glm::vec3 from, glm::vec3 to, float t);
	glm::vec2 Vec2Lerp(glm::vec2 from, glm::vec2 to, float t);
	glm::vec4 Vec4Lerp(glm::vec4 from, glm::vec4 to, float t);

}

#endif