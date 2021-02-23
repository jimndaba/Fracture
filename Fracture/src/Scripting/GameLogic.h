#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <memory>
#include <string>
#include "Component/ComponentManager.h"
#include "Event/PhysicsEvents.h"

namespace Fracture
{
	class Entity;

	class GameLogic
	{

	public:
		GameLogic() {};
		~GameLogic() {};

		bool isStarted = false;

		virtual void onStart() = 0;
		virtual void onUpdate(float dt) = 0;
		virtual void onLateUpdate(float dt) = 0;
		virtual void onCollision(CollisionEvent* collision) = 0;

		//Scripting helper functions
		template<class T>
		static std::shared_ptr<T> GetComponent(UUID id);

		template<class T>
		static std::shared_ptr<T> GetComponent(std::string entityName);

		void destroy(UUID id);
		void destroy(std::shared_ptr<Entity> entity);

	};

	template<class T>
	inline std::shared_ptr<T> GameLogic::GetComponent(UUID id)
	{
		return ComponentManager::GetComponent<T>(entityID);
	}

	template<class T>
	inline std::shared_ptr<T> GameLogic::GetComponent(std::string entityName)
	{
		return ComponentManager::GetComponent<T>(entityName);
	}

}

#endif
