#pragma once
#ifndef CUBESCRIPT_H
#define CUBESCRIPT_H

#include "Fracture.h"

namespace Fracture
{

	class CubeScript: public GameLogic
	{

	public:
		CubeScript(int id);

		virtual void onStart();
		
		
		virtual void onUpdate(float dt);
		

		virtual void onLateUpdate();
	
		void onCollision(CollisionEvent* collision);

	private:

		std::shared_ptr<TransformComponent> transform;
		int m_id;
		glm::vec3 targetPosition;


	};

}

#endif