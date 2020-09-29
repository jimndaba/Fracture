#pragma once
#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H
#include "Entity.h"
#include "IDManager.h"

namespace Fracture
{

	class DirectLight :public Entity
	{
	public:
		DirectLight(int id);
		~DirectLight();
	};
}

#endif