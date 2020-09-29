#pragma once
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "IDManager.h"
#include <memory>

namespace Fracture
{

	class EntityManager
	{

	public:
		EntityManager();
		~EntityManager();

		static Entity CreateEntity()
		{
			return Entity(IDManager::GetID());
		}

		static std::shared_ptr<Entity> Create_Entity_ptr()
		{
			return std::shared_ptr<Entity>(new Entity(IDManager::GetID()));
		}
	private:

	};

}

#endif