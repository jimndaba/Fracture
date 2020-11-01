#pragma once
#ifndef PHYSICSUTILITY_H
#define PHYSICSUTILITY_H

#include "Bullet/include/btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Fracture
{


	static btVector3 ToBtVec3(glm::vec3 vec3)
	{
		return btVector3(vec3.x,vec3.y,vec3.z);
	}

	static btQuaternion ToBtQuat(glm::vec3 vec3)
	{
		glm::quat rot = glm::quat(vec3);
		return btQuaternion(rot.x, rot.y, rot.z, rot.w);
	}


}

#endif