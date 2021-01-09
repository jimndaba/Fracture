#ifndef FRUSTUMPLANE_H
#define FRUSTUMPLANE_H

#include "glm/glm.hpp"

namespace Fracture
{

	class FrustumPlane
	{
		enum Where { COPLANAR, FRONT, BACK };

	public:
		FrustumPlane(void);
		FrustumPlane(const glm::vec3& N, const glm::vec3& p);
		~FrustumPlane(void);

		static FrustumPlane FromPoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
		Where Classify(const glm::vec3& p);
		float GetDistance(const glm::vec3& p);


		glm::vec3 N;
		float d;

	};

}


#endif