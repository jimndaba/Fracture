#include "ScriptComponent.h"
#include "Scripting/GameLogic.h"

Fracture::ScriptComponent::ScriptComponent(uint32_t id, std::shared_ptr<GameLogic> script):Component(id,ComponentType::Script),m_script(script)
{
}

Fracture::ScriptComponent::~ScriptComponent()
{
}

void Fracture::ScriptComponent::onStart()
{
}

std::shared_ptr<Fracture::GameLogic> Fracture::ScriptComponent::GetScript()
{
	if (m_script)
		return m_script;
}
