#pragma once
#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "Component.h"
#include <memory>

namespace Fracture
{
	class GameLogic;

	class ScriptComponent:public Component
	{

	public:
		ScriptComponent(uint32_t id, std::shared_ptr<GameLogic>  script);
		~ScriptComponent();


		virtual void onStart();

		std::shared_ptr<GameLogic> GetScript();

	private:

		std::shared_ptr<GameLogic> m_script;

	};


}

#endif