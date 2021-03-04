#include "ScriptComponent.h"

Fracture::ScriptComponent::ScriptComponent(UUID id, std::shared_ptr<GameLogic> script):Component(id),m_script(script)
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

void Fracture::ScriptComponent::Accept(ISceneProbe* visitor)
{
}

nlohmann::json Fracture::ScriptComponent::serialise(const std::shared_ptr<ComponentSerialiser>& visitor)
{
	return visitor->visitScriptComponent(*this);
}
