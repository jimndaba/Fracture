#pragma once
#ifndef RAY_H
#define RAY_H

namespace Fracture
{

	class Ray
	{
	public:
		Ray(const glm::vec3 start, glm::vec3 direction);


		glm::vec3 GetEndPoint(float t)
		{
			endpoint = origin + direction * t;
			length = t;
			return endpoint;
		}

		glm::vec3 origin = glm::vec3(0.0f);
		glm::vec3 endpoint = glm::vec3(0.0f);
		glm::vec3 direction = glm::vec3(0.0f);
		glm::vec3 invdirection = glm::vec3(0.0f);
		float length = 1000.0f;


	private:


	};

	struct RayHit
	{
		UUID ID;
	};



}


#endif