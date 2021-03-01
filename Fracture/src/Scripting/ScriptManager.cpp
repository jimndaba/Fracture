#include "ScriptManager.h"
#include "GameLogic.h"
#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"

sol::state Fracture::ScriptManager::lua;


Fracture::ScriptManager::ScriptManager()
{
	Eventbus::Subscribe(this, &Fracture::ScriptManager::OnCollision);
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.set_function("LogMessage", Fracture::ScriptManager::Log);
	lua.set_function("GetAxis", Fracture::ScriptManager::GetAxis);

	lua.do_file("content/scripts/test.lua");

	sol::function update = lua["Update"];
	update();

}

Fracture::ScriptManager::~ScriptManager()
{

}

void Fracture::ScriptManager::AddScript(std::shared_ptr<GameLogic> script)
{
		m_scripts.push_back(script);	
}

void Fracture::ScriptManager::RemoveScript()
{
}

void Fracture::ScriptManager::clear()
{
	m_scripts.clear();
}

void Fracture::ScriptManager::Log(const std::string& message)
{
	FRACTURE_INFO(message);	
}

float Fracture::ScriptManager::GetAxis(const std::string& message)
{
	if (message == "vertical")
	{
		return 10.0f;
	}

	if (message == "horizontal")
	{
		return 100.0f;
	}

	return 2.0f;
}

void Fracture::ScriptManager::onStart()
{
	for (auto script : m_scripts)
	{
		script->onStart();
	}

}

void Fracture::ScriptManager::OnUpdate(float dt)
{
	ProfilerTimer timer("Script Update");
	for (auto script : m_scripts)
	{
		if (!script->isStarted)
		{
			script->onStart();
		}
		script->onUpdate(dt);
	}

}

void Fracture::ScriptManager::OnCollision(CollisionEvent* collision)
{
	for (auto script : m_scripts)
	{
		script->onCollision(collision);
	}
}

void Fracture::ScriptManager::onEndFrame()
{
	clear();
}

void Fracture::ScriptManager::start()
{
}

void Fracture::ScriptManager::update(float dt)
{
}

