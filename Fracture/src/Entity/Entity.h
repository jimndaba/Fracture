#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

namespace Fracture
{
	class Entity
	{
	public:
		Entity();
		~Entity();
		uint32_t Id;
		std::string name;

		operator bool() const
		{
			return true;
		}
	private:

	};

	class EntityInstance
	{
	public:
		uint32_t EntityID;
		uint32_t InstanceID;
	};

}

#endif
