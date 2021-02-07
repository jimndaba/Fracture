#pragma once
#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "Component.h"


namespace Fracture
{
	class GameLogic;

	class ScriptComponent:public Component
	{

	public:
		ScriptComponent(uint32_t id, std::shared_ptr<GameLogic>  script);
		~ScriptComponent() = default;

		ScriptComponent(const ScriptComponent& component, uint32_t id) :Component(id, ComponentType::Script)
		{
			m_script = component.m_script;
		}

		virtual void onStart();
		std::shared_ptr<GameLogic> GetScript();
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<ScriptComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<ScriptComponent>(this->clone_impl(entityID));
		}

	
	private:

		virtual ScriptComponent* clone_impl(uint32_t entityID) const override
		{
			return new ScriptComponent(*this, entityID);
		}
		std::shared_ptr<GameLogic> m_script;

	};


}

#endif