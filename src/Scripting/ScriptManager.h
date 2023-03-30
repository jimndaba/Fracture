#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "sol/sol.hpp"


namespace Fracture
{
	class LuaScript;
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

		void Reload(LuaScript* mscript);

		static void LoadScript(const std::shared_ptr<LuaScript>& mscript);

		static sol::state* GetState();
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