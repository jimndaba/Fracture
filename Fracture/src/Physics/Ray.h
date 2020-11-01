#pragma once
#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"

namespace Fracture
{
	class Ray
	{

	public:
		Ray(glm::vec3 origin, glm::vec3 direction) :m_origin(origin), m_direction(direction)
		{
			m_invdirection = glm::vec3(1.0f / m_direction.x, 1.0f / m_direction.y, 1.0f / m_direction.z);
		}

		glm::vec3 GetOrigin()
		{
			return m_origin;
		}
		glm::vec3 GetEndPoint(float t)
		{
			m_endpoint = m_origin + m_direction * t;
			m_length = t;
			return m_endpoint;
		}
		glm::vec3 GetDirection()
		{
			return m_direction;
		}
		glm::vec3 GetInverseDirection()
		{
			return m_invdirection;
		}
		float GetLength()
		{
			return m_length;
		}

		void SetOrigin(glm::vec3 start)
		{
			m_origin = start;
		}
		void SetDirection(glm::vec3 dir)
		{
			m_direction = dir;
		}

	private:
		glm::vec3 m_origin = glm::vec3(0.0f);
		glm::vec3 m_endpoint = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f);
		glm::vec3 m_invdirection = glm::vec3(0.0f);
		float m_length = 0.0f;

	};


}
#endif