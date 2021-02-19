#pragma once
#ifndef SUNLIGHT_H
#define SUNLIGHT_H

#include "ILight.h"

namespace Fracture
{
	class SunLight :public ILight
	{
	public:
		SunLight()
		{
		}

		~SunLight()
		{
			m_ambient = glm::vec4(0.0f);
			m_diffuse = glm::vec4(0.0f);
			m_specular = glm::vec4(0.0f);
			m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
		}

		virtual void SetAmbient(glm::vec4 ambient)
		{
			m_ambient = ambient;
		};
		virtual void SetDiffuse(glm::vec4 diffuse)
		{
			m_diffuse = diffuse;
		};
		virtual void SetSpecular(glm::vec4 specular)
		{
			m_specular = specular;
		};

		virtual void SetDirection(glm::vec3 direction)
		{
			m_direction = direction;
		};

		void SetRadiance(glm::vec3 value)
		{
			radiance = value;
		}

		virtual glm::vec4 GetAmbient()
		{
			return m_ambient;
		};
		virtual glm::vec4 GetDiffuse()
		{
			return m_diffuse;
		};
		virtual glm::vec4 GetSpecular()
		{
			return m_specular;
		};

		virtual glm::vec3 GetDirection()
		{
			return m_direction;
		};

		virtual glm::vec3 GetRadiance()
		{
			return radiance;
		}

		virtual LightType GetLightType()
		{
			return LightType::Sun;
		}

	private:
		glm::vec4 m_ambient = glm::vec4(0.6f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(1.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 radiance = glm::vec3(1.0f);
	};



}

#endif 