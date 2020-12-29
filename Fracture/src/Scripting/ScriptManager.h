#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <memory>
#include <vector>
#include "Event/PhysicsEvents.h"

#include <fstream>
#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Fracture
{

	class GameLogic;


	class ScriptManager
	{
		
	public:
		ScriptManager();
		~ScriptManager();

		void AddScript(std::shared_ptr<GameLogic> script);
		void RemoveScript();

		void clear();

		void onStart();
		void OnUpdate(float dt);

		void OnCollision(CollisionEvent* collision);

		void onEndFrame();

		bool Start = true;

	/// <summary>
	/// Lua Script functions
	/// </summary>
	public:

		void start();
		void update(float dt);

	private:

		std::vector<std::shared_ptr<GameLogic>> m_scripts;
		sol::state lua;
	};

}

#endif