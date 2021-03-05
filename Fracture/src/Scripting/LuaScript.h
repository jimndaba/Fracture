#pragma once
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#define GLM_FORCE_NO_CTOR_INIT 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sol/sol.hpp>

namespace Fracture
{
	class Texture;

	enum PROPERTY_TYPE
	{
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
			Texture* texture;
		};

		ScriptProperty() {};
		~ScriptProperty() {};
	};

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
		std::string GetName();

		void BindFunction(sol::state& state);
		void BindProperties(sol::state& state);

		static std::shared_ptr<LuaScript> Create(const std::string& name, const std::string& path);

		std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ScriptProperty>>> GetProperties();
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

		std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ScriptProperty>>> m_Properties;
	};
}

#endif