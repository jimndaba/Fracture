#pragma once
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "IDManager.h"
#include <memory>

namespace Fracture
{
	class UUID;
	class EntityManager
	{

	public:
		EntityManager();
		~EntityManager() = default;		
		

		template <class T>
		static std::shared_ptr<T> CreateEntity();

		static std::shared_ptr<Entity> CreateEntity(UUID id);
		
	private:

	};

	template<class T>
	inline std::shared_ptr<T> EntityManager::CreateEntity()
	{
		auto entity = std::make_shared<T>();
		return entity;
	}


}

#endif