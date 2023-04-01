
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


}

#endif