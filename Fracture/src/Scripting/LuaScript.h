#pragma once
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <sol/sol.hpp>

namespace Fracture
{

	class LuaScript
	{
	public:
		LuaScript(const std::string& name, const std::string& path);

		void onStart(sol::state& state);
		void onExit(sol::state& state);
		void onUpdate(sol::state& state,float dt);
		void onLateUpate(sol::state& state,float dt);
		void onFixedUpdate(sol::state& state,float dt);
		void onCollision(sol::state& state);
		void onTrigger(sol::state& state);

		void Load(sol::state& state);
		void Reload(sol::state& state);

		std::string GetFilepath();


		void BindFunction(sol::state& state);

		static std::shared_ptr<LuaScript> Create(const std::string& name, const std::string& path);
	private:
		std::string m_name;
		std::string m_filepath;
		
		std::shared_ptr<sol::protected_function> m_onStart;
		std::shared_ptr<sol::protected_function> m_onExit;
		std::shared_ptr<sol::protected_function> m_onUpdate;
		std::shared_ptr<sol::protected_function> m_onLateUpdate;
		std::shared_ptr<sol::protected_function> m_onFixedUpdate;
		std::shared_ptr<sol::protected_function> m_onCollision;
		std::shared_ptr<sol::protected_function> m_onTrigger;
	};
}

#endif