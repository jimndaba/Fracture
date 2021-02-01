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

		virtual void onStart();
		std::shared_ptr<GameLogic> GetScript();
		void Accept(ISceneProbe* visitor) override;

	private:

		std::shared_ptr<GameLogic> m_script;

	};


}

#endif