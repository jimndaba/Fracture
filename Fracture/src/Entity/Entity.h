#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "UUID.h"

namespace Fracture
{
	class Entity
	{
	public:
		Entity();
		Entity(UUID id);
		
		~Entity();
		UUID GetId();

		operator bool() const
		{
			return true;
		}
	private:
		UUID m_ID;
	};

}

#endif
