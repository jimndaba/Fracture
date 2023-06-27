#pragma once
#ifndef PHYSICSEVENTS_H
#define PHYSICSEVENTS_H

#include "EventSystem/Event.h"

namespace Fracture
{
	struct SceneManager;
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

	struct OnAddActorEvent : public Event
	{
		OnAddActorEvent(SceneManager* sm,UUID entity);

		SceneManager* manager;
		UUID id;


		const char* Name() { return "OnCollision"; };
	};


}

#endif

