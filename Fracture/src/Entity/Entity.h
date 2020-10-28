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
		Entity(uint32_t id);
		~Entity();
		uint32_t Id;

		operator bool() const
		{
			return true;
		}
	private:

	};

}

#endif
