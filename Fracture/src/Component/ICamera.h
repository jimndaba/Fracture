#pragma once
#ifndef ICAMERA_H
#define ICAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fracture
{
	enum class Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class ICamera
	{
	public:
		virtual glm::mat4 getProjectionMatrix(int width,int height) = 0;
		virtual glm::mat4 getViewMatrix() = 0;
		virtual glm::vec3 getPosition() = 0;
		virtual void Move(Camera_Movement td, float dt) = 0;
		virtual void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true) = 0;
		virtual void onUpdate(float dt) = 0;
	};
}

#endif