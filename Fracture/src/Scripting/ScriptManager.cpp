#include "ScriptManager.h"
#include "GameLogic.h"
#include "Profiling/Profiler.h"
#include "Event/Eventbus.h"



Fracture::ScriptManager::ScriptManager()
{
	Eventbus::Subscribe(this, &Fracture::ScriptManager::OnCollision);

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

