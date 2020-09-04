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

		template<class T>
		std::shared_ptr<T> GetComponent(int entityID);

		template<class T>
		std::shared_ptr<T> GetComponent(std::string entityName);

	};

	template<class T>
	inline std::shared_ptr<T> GameLogic::GetComponent(int entityID)
	{
		return ComponentManager::GetComponent<T>(entityID);;
	}

	template<class T>
	inline std::shared_ptr<T> GameLogic::GetComponent(std::string entityName)
	{
		return std::shared_ptr<T>();
	}

}

#endif
