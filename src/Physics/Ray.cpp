#include "FracturePCH.h"
#include "Ray.h"

Fracture::Ray::Ray(const glm::vec3 start, glm::vec3 direction) :
	origin(start),
	direction(direction),
	invdirection(glm::vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z))
{
}