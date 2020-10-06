#include "ScriptComponent.h"
#include "Scripting/GameLogic.h"

Fracture::ScriptComponent::ScriptComponent(int id, std::shared_ptr<GameLogic> script):Component(id,ComponentType::Script),m_script(script)
{
}

Fracture::ScriptComponent::~ScriptComponent()
{
}

void Fracture::ScriptComponent::onAttach()
{
}

void Fracture::ScriptComponent::onDettach()
{
}

std::shared_ptr<Fracture::GameLogic> Fracture::ScriptComponent::GetScript()
{
	if (m_script)
		return m_script;
}
