#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "sol/sol.hpp"


namespace Fracture
{
	class LuaScript;
	struct LuaScriptRegistry;
	struct Entity;

	class ScriptManager
	{
	public:
		ScriptManager();

		void BindLog(sol::state& L);
		void BindCore(sol::state& lua);
		void BindFunctions(sol::state& L);
		void BindInput(sol::state& L);
		void BindMaths(sol::state& L);
		void BindApplication(sol::state& L);

		void Init();
		void onStart();
		void onExit();
		void onUpdate(float dt);
		void Shutdown();

		static void RegisterScript(const LuaScriptRegistry& reg);

		void Reload(LuaScript* mscript);
		static void LoadScript(const std::shared_ptr<LuaScript>& mscript);
		static std::shared_ptr<LuaScript> GetInstanceOfScript(const UUID& id);

		static void CreateNewScript(const LuaScriptRegistry& reg);

		static sol::state* GetState();

		bool HasScript(const UUID& script);

		static std::unordered_map<UUID, std::shared_ptr<LuaScript>> mScripts;
		static std::map<UUID, LuaScriptRegistry> mScriptRegister;

	private:



		static sol::state* lua;
		template<class T>
		static T* GetComponentByType(const Fracture::UUID& id);
		static Fracture::Entity* GetEntity(const std::string& name);

	};

	template<class T>
	inline T* ScriptManager::GetComponentByType(const UUID& id)
	{
		/*
		auto component = Application::CurrentScene()->GetComponent<T>(id);
		if (component)
		{
			return component;
		}
		else
		{
			FRACTURE_ERROR("Couldnt find component");
			return nullptr;
		}
		*/
		return nullptr;
	}

}

#endif