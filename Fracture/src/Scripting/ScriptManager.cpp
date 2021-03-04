#include "ScriptManager.h"
#include "Logging/Logger.h"
#include "LuaScript.h"

Fracture::ScriptManager::ScriptManager()
{

	lua.open_libraries(sol::lib::base);

	BindLog(lua);
	
	script = LuaScript::Create("CameraController", "content/scripts/");
	script->Load(lua);

	script2 = LuaScript::Create("PlayerController", "content/scripts/");
	script2->Load(lua);
}

void Fracture::ScriptManager::BindLog(sol::state& L)
{
	auto log = L.create_table("Debug");

	log.set_function("log", [&](sol::this_state s, std::string message) {
		FRACTURE_INFO(message);
		});

	log.set_function("trace", [&](sol::this_state s, std::string message) {
		FRACTURE_TRACE(message);
		});

	log.set_function("error", [&](sol::this_state s, std::string message) {
		FRACTURE_ERROR(message);
		});

	log.set_function("warn", [&](sol::this_state s, std::string message) {
		FRACTURE_WARN(message);
		});

	log.set_function("critical", [&](sol::this_state s, std::string message) {
		FRACTURE_CRITICAL(message);
		});
}

void Fracture::ScriptManager::BindInput(sol::state& L)
{
	auto input = L.create_table("Debug");
	//
	//log.set_function("log", [&](sol::this_state s, std::string message) {
	//	FRACTURE_INFO(message);
	//	});
	//
	//log.set_function("trace", [&](sol::this_state s, std::string message) {
	//	FRACTURE_TRACE(message);
	//	});
	//
	//log.set_function("error", [&](sol::this_state s, std::string message) {
	//	FRACTURE_ERROR(message);
	//	});
	//
	//log.set_function("warn", [&](sol::this_state s, std::string message) {
	//	FRACTURE_WARN(message);
	//	});
	//
	//log.set_function("critical", [&](sol::this_state s, std::string message) {
	//	FRACTURE_CRITICAL(message);
	//		});
}

void Fracture::ScriptManager::onStart()
{
	script->onStart(lua);
}

void Fracture::ScriptManager::onExit()
{
	script->onExit(lua);
}

void Fracture::ScriptManager::onUpdate(float dt)
{
	script->Reload(lua);
	script->onUpdate(lua,dt);

	script2->Reload(lua);
	script2->onUpdate(lua, dt);
}
