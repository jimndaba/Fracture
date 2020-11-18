#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <memory>
#include <vector>
#include "Event/PhysicsEvents.h"

#include "sol/sol.hpp"
#include <fstream>
#include <iostream>

extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}




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

		static sol::state* GetState();
	private:

		std::vector<std::shared_ptr<GameLogic>> m_scripts;
		static sol::state* lua;

	};

}

#endif