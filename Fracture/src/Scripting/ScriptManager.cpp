#include "ScriptManager.h"
#include "GameLogic.h"
#include "Profiling/Profiler.h"

Fracture::ScriptManager::ScriptManager()
{
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
	
	for (auto script : m_scripts)
	{
		script->onUpdate(dt);
	}
	clear();
}
