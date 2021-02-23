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
		ScriptComponent(UUID id, std::shared_ptr<GameLogic>  script);
		~ScriptComponent() = default;

		ScriptComponent(const ScriptComponent& component, UUID id) :Component(id)
		{
			m_script = component.m_script;
		}

		virtual void onStart();
		std::shared_ptr<GameLogic> GetScript();
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<ScriptComponent> clone(UUID id) const
		{
			return std::shared_ptr<ScriptComponent>(this->clone_impl(id));
		}

	
	private:

		virtual ScriptComponent* clone_impl(UUID id) const override
		{
			return new ScriptComponent(*this, id);
		}
		std::shared_ptr<GameLogic> m_script;

	};


}

#endif