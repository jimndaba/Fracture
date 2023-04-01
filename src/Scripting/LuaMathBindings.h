#pragma once
#ifndef LUABINDMATH_H
#define LUABINDMATH_H

#include "sol/sol.hpp"

namespace LuaBindGLM
{
	void BindVec2(sol::state& lua);
	void BindVec3(sol::state& lua);
	void BindVec4(sol::state& lua);

	void BindMat3(sol::state& lua);
	void BindMat4(sol::state& lua);

	void BindQuat(sol::state& lua);

}

#endif