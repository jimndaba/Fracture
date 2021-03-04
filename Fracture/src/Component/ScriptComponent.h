#pragma once
#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "Component.h"


namespace Fracture
{
	class GameLogic;
	class LuaScript;

	class ScriptComponent:public Component
	{

	public:
		ScriptComponent(UUID id, std::shared_ptr<LuaScript>  script);
		~ScriptComponent() = default;

		ScriptComponent(const ScriptComponent& component, UUID id) :Component(id)
		{
			m_script = component.m_script;
		}

		virtual void onStart();

		void SetScript(const std::shared_ptr<LuaScript>&  script);
		std::shared_ptr<LuaScript> GetScript();

		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<ScriptComponent> clone(UUID id) const
		{
			return std::shared_ptr<ScriptComponent>(this->clone_impl(id));
		}

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

		static std::shared_ptr<ScriptComponent> Create(UUID id);
		static std::shared_ptr<ScriptComponent> Create(UUID id,const std::shared_ptr<LuaScript>&  script);

	private:

		virtual ScriptComponent* clone_impl(UUID id) const override
		{
			return new ScriptComponent(*this, id);
		}
		std::shared_ptr<LuaScript> m_script;

	};


}

#endif