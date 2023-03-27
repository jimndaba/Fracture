#pragma once
#ifndef PHYSICSEVENTS_H
#define PHYSICSEVENTS_H

#include "EventSystem/Event.h"

namespace Fracture
{
	struct CollisionContext
	{
		Fracture::UUID entity;
		std::string Tag;
	};

	struct OnCollisionEvent : public Event
	{
		OnCollisionEvent(CollisionContext a, CollisionContext b);

		CollisionContext EntityA;
		CollisionContext EntityB;

		const char* Name() { return "OnCollision"; };
	};


}

#endif

