#pragma once
#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include "assimp/Importer.hpp"

namespace Fracture {
	
	namespace Math 
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

		glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix);
	}

}

#endif 