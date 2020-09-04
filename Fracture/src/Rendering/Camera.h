#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Fracture
{
	class Camera
	{
	public:

		Camera() {};
		~Camera() {};

		virtual glm::mat4 getViewMatrix() = 0;
		virtual glm::mat4 getProjectionMatrix() = 0;

	};

}

#endif
