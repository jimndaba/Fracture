#pragma once
#ifndef DIRECTLIGHTCOMPONENT_H
#define DIRECTLIGHTCOMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace Fracture
{

	class DirectLightComponent : public Component
	{

		glm::vec3 m_direction = glm::vec3(0.0f);
		glm::vec3 m_ambient = glm::vec3(0.0f);
		glm::vec3 m_diffuse = glm::vec3(0.0f);
		glm::vec3 m_specular = glm::vec3(0.0f);

	public:
		DirectLightComponent(uint32_t id);
		~DirectLightComponent();


		virtual void onStart();

		glm::vec3 GetLightDirection()
		{
			return m_direction;
		}

		glm::vec3 GetLightAmbient()
		{
			return m_ambient;
		}

		glm::vec3 GetLightDiffuse()
		{
			return m_diffuse;
		}

		glm::vec3 GetLightSpecular()
		{
			return m_specular;
		}

		void SetDirection(glm::vec3 direction);
		void SetAmbient(glm::vec3 ambient);
		void SetDiffuse(glm::vec3 diffuse);
		void SetSpecular(glm::vec3 specular);

	};

}

#endif
