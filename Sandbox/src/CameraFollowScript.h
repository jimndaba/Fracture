#pragma once
#ifndef CAMERAFOLLOWSCRIPT_H
#define CAMERAFOLLOWSCRIPT_H

#include "Fracture.h"

namespace Fracture
{

	class CameraFollowScript : public GameLogic
	{

	public:
		CameraFollowScript(uint32_t id);

		virtual void onStart();


		virtual void onUpdate(float dt);


		virtual void onLateUpdate(float dt);

		glm::vec3 offset;
	    float smoothTime = 0.3F;
		uint32_t target;

	private:
		std::shared_ptr<CameraControllerComponent> camera;
		std::shared_ptr<TransformComponent> tagerTtransform;
		uint32_t m_id;


	};

}

#endif