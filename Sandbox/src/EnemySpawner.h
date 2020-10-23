#pragma once
#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <cstdlib>
#include <ctime>

#include "Fracture.h"

namespace Fracture
{

	class EnemySpawner : public GameLogic
	{

	public:
		EnemySpawner(uint32_t id,std::shared_ptr<Entity> root);
		~EnemySpawner();


		virtual void onStart();


		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		void onCollision(CollisionEvent* collision);

		float spawnRate = 5;

		float counter = 0.0f;

	private:
		std::shared_ptr<Entity> m_root;
	};


}

#endif