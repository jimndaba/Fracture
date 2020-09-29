#pragma once
#ifndef INSTANCEPOOL_H
#define INSTANCEPOOL_H

#include "Entity/Entity.h"
#include <vector>
#include <string>
#include <memory>

namespace Fracture
{

	class InstancePool
	{
	public:
		InstancePool(std::string name);
		~InstancePool();

		std::string Name;


		void AddInstance(std::shared_ptr<EntityInstance> instance);

		void Clear();

		std::vector<std::shared_ptr<EntityInstance>> GetInstances();

	private:
		std::vector<std::shared_ptr<EntityInstance>> m_instances;

	};


}

#endif