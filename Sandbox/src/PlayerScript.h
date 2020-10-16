#pragma once

#ifndef PLAYERSCRIPT_H
#define PLAYERSCRIPT_H

#include "Fracture.h"

namespace Fracture
{

	class PlayerScript : public GameLogic
	{

	public:
		PlayerScript(uint32_t id);

		virtual void onStart();


		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		
		float smoothTime = 0.3F;
		float playerSpeed = 1.0f;
		glm::vec3 position = glm::vec3();
	private:
		std::shared_ptr<TransformComponent> transform;
		uint32_t m_id;


	};

}

#endif