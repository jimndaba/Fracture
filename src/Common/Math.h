#pragma once
#ifndef MATH_H
#define MATH_H

#include "Frustum.h"

namespace Fracture {

	struct AABB;
	class FrustumPlane;

	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

		static inline glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& from)
		{
			glm::mat4 to;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
			to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
			to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
			to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
			return to;

			//return glm::transpose(glm::make_mat4(&from.a1));
		}

		static inline glm::vec3 ConvertToDegress(const glm::vec3& radians)
		{
			return glm::degrees(radians);
		}

		static float Max(const float& a, const float& b, const float& c)
		{
			return fmaxf(a, fmaxf(b, c));
		}
		static float Min(const float& a, const float& b, const float& c)
		{
			return fminf(a, fminf(b, c));
		}

	
		static bool IntersectionAABBPlane(const glm::vec3& min, const glm::vec3& max, const FrustumPlane& plane)
		{
			glm::vec3 center = (max + min) * 0.5f;
			glm::vec3 extents = max - center;

			float r = extents.x * glm::abs(plane.normal.x) + extents.y * glm::abs(plane.normal.y) + extents.z * glm::abs(plane.normal.z);
			float s = glm::dot(plane.normal, center) - plane.distance;

			return glm::abs(s) <= r;
		}
		
	}

}

#endif