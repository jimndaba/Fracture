#pragma once
#ifndef PLAYERSHOOT_H
#define PLAYERSHOOT_H

#include "Fracture.h"

namespace Fracture
{

	class PlayerShootScript :public GameLogic
	{

	public:
		PlayerShootScript(uint32_t id, std::shared_ptr<Entity> root);
		~PlayerShootScript();


		virtual void onStart();

		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		void onCollision(CollisionEvent* collision);

		float cooldown = 0.5f;
		float currenttime = 0;
		bool canShoot = true;

		uint32_t m_id;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Entity> m_root;
	};

}

#endif