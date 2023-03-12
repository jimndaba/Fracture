#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Common/UUID.h"

namespace Fracture
{

	struct Entity
	{
		Entity(UUID id = UUID());
		UUID ID;
	};



}


#endif
