#include "ScriptComponent.h"
#include "Scripting/LuaScript.h"

Fracture::ScriptComponent::ScriptComponent(UUID id, std::shared_ptr<LuaScript> script):Component(id),m_script(script)
{
}

void Fracture::ScriptComponent::onStart()
{
}

void Fracture::ScriptComponent::SetScript(const std::shared_ptr<LuaScript>& script)
{
	m_script = script;
}

std::shared_ptr<Fracture::LuaScript> Fracture::ScriptComponent::GetScript()
{
	if (m_script)
		return m_script;
	else return nullptr;
}

void Fracture::ScriptComponent::Accept(ISceneProbe* visitor)
{
}

nlohmann::json Fracture::ScriptComponent::serialise(const std::shared_ptr<ComponentSerialiser>& visitor)
{
	return visitor->visitScriptComponent(*this);
}

std::shared_ptr < Fracture::ScriptComponent > Fracture::ScriptComponent::Create(UUID id)
{
	std::shared_ptr<LuaScript> m_script = nullptr;
	return std::make_shared<ScriptComponent>(id,m_script);
}

std::shared_ptr<Fracture::ScriptComponent> Fracture::ScriptComponent::Create(UUID id, const std::shared_ptr<LuaScript>& script)
{
	return std::make_shared<ScriptComponent>(id,script);
}
