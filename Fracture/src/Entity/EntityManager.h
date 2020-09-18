#pragma once
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
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
			return Entity(getID());
		}

		static std::shared_ptr<Entity> Create_Entity_ptr()
		{
			return std::shared_ptr<Entity>(new Entity(getID()));
		}

		static int getID()
		{
			int current = m_currentID;
			m_currentID += 1;
			return current;
		}

	private:

		static int m_currentID;



	};

}

#endif