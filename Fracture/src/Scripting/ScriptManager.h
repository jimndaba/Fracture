#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>




namespace Fracture
{
	class LuaScript;
	class ScriptManager
	{
	public:
		ScriptManager();

		void BindLog(sol::state& L);
		void BindInput(sol::state& L);
		void BindApplication(sol::state& L);

		void onStart();
		void onExit();
		void onUpdate(float dt);

	private:
		sol::state lua;
		std::shared_ptr<LuaScript> script;
		std::shared_ptr<LuaScript> script2;
	};

}

#endif