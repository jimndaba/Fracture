#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"
#include <vector>
#include <math.h>
#include <memory>

#include "Entity/Entity.h"
#include "Game/GameWindow.h"
#include "Component/CameraControllerComponent.h"


namespace Fracture
{
	class Camera : public Entity
	{
	public:
		Camera();
		~Camera() {};

		std::shared_ptr<CameraControllerComponent> camerController;
	};

}

#endif
