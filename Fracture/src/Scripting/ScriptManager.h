#pragma once
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <memory>
#include <vector>
#include "Event/PhysicsEvents.h"


namespace Fracture
{
	class GameLogic;

	class ScriptManager
	{

	public:
		ScriptManager();
		~ScriptManager();

		void AddScript(std::shared_ptr<GameLogic> script);
		void RemoveScript();

		void clear();

		void onStart();
		void OnUpdate(float dt);

		void OnCollision(CollisionEvent* collision);

		void onEndFrame();

		bool Start = true;

	private:

		std::vector<std::shared_ptr<GameLogic>> m_scripts;


	};

}

#endif