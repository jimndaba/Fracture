#pragma once
#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>


namespace Fracture {
	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);



}

}

#endif 