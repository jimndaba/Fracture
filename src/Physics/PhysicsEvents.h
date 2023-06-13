#pragma once
#ifndef PHYSICSEVENTS_H
#define PHYSICSEVENTS_H

#include "EventSystem/Event.h"

namespace Fracture
{
	struct CollisionContext
	{
		Fracture::UUID entity;
		Fracture::UUID other;
		std::string Tag;
	};

	struct OnCollisionEvent : public Event
	{
		OnCollisionEvent(CollisionContext c);

		CollisionContext Collision;


		const char* Name() { return "OnCollision"; };
	};


}

#endif

