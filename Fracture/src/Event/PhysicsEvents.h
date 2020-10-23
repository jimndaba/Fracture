#pragma once
#ifndef PHYSICSEVENTS_H
#define PHYSICSEVENTS_H

#include "Event.h"
#include "Physics/Collisions.h"
#include "Logging/Logger.h"

namespace Fracture
{
	struct CollisionEvent : public Event
	{

		CollisionEvent(int a, int b):entityA(a),entityB(b)
		{
		
		}

		int entityA;
		int entityB;

		virtual void Handled() {}
	};

}

#endif 