#pragma once
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "sol/sol.hpp"
#include "Assets/AssetRegistries.h"

namespace Fracture
{
	struct CollisionContext;

	enum class PROPERTY_TYPE
	{
		UUID,
		STRING,
		BOOL,
		INT,
		FLOAT,
		TEXTURE2D,
		TEXTURECUBEMAP,
		VEC2,
		VEC3,
		VEC4,
		COLOR3,
		COLOR4,
		ARRAY
	};

	struct ScriptProperty
	{
		PROPERTY_TYPE Type;
		std::string Name;
		union
		{
			std::string String;
			bool       Bool;
			int        Int;
			float      Float;
			glm::vec2 Vec2;
			glm::vec3 Vec3;
			glm::vec4 Vec4;
			glm::vec3 Color3;
			glm::vec4 Color4;
			Fracture::UUID ID;
			std::vector<UUID> Array;
		};

		ScriptProperty() {};

		ScriptProperty(const ScriptProperty& other) {
		}

		inline bool operator == (const ScriptProperty& other) const {
			return Name == other.Name;
		}


		~ScriptProperty() {};
	};

	class LuaScript
	{
	public:
		LuaScript(const LuaScriptRegistry& reg);

		void OnStart(sol::state& state,const Fracture::UUID& entity);
		void OnExit(sol::state& state, const Fracture::UUID& entity);
		void OnUpdate(sol::state& state, float dt, const Fracture::UUID& entity);
		void OnLateUpate(sol::state& state, float dt, const Fracture::UUID& entity);
		void OnFixedUpdate(sol::state& state,float dt, const Fracture::UUID& entity);
		void OnCollision(sol::state& state, const Fracture::CollisionContext& cntxt);
		void OnTrigger(sol::state& state, const Fracture::UUID& entity);

		void DoScript(sol::state& state);
		void Load(sol::state& state);
		void Reload(sol::state& state);

		bool IsStarted()const;
		void IsStarted(bool started);

		void BindFunctions(sol::state& state);
		void BindProperties(sol::state& state);

		void OnPropertyUpdate(sol::state& state, const Fracture::ScriptProperty& prop);

		
	std::unordered_map <std::string ,std::shared_ptr<Fracture::ScriptProperty>> GetProperties();

		LuaScriptRegistry Description;
		std::unordered_map <std::string, std::shared_ptr<Fracture::ScriptProperty>> m_Properties;

	private:
		std::string m_filepath;
		bool isStarted = false;

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