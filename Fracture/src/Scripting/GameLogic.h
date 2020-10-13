#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <memory>
#include <string>
#include "Component/ComponentManager.h"

namespace Fracture
{

	class GameLogic
	{

	public:
		GameLogic() {};
		~GameLogic() {};

		virtual void onStart() = 0;
		virtual void onUpdate(float dt) = 0;
		virtual void onLateUpdate(float dt) = 0;

		template<class T>
		static std::shared_ptr<T> GetComponent(uint32_t entityID);

		template<class T>
		static std::shared_ptr<T> GetComponent(std::string entityName);

	};

	template<class T>
	inline std::shared_ptr<T> GameLogic::GetComponent(uint32_t entityID)
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
