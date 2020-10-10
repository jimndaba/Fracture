#pragma once
#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "glm/glm.hpp"
#include "Component.h"
#include <memory>

namespace Fracture
{
	class PointLightComponent : public Component
	{

		glm::vec3 ambient = glm::vec3(0.0f);
		glm::vec3 diffuse = glm::vec3(0.0f);
		glm::vec3 specular = glm::vec3(0.0f);
		float constant = 0.0f;
		float linear = 0.0f;
		float quadratic = 0.0f;

	public:
		PointLightComponent(uint32_t id);
		~PointLightComponent();


		virtual void onStart();

		void SetPosition(glm::vec3 pos);



		glm::vec3 GetPosition();

		glm::vec3 GetAmbient()
		{
			return ambient;
		}
		glm::vec3 GetDiffuse()
		{
			return diffuse;
		}
		glm::vec3 GetSpecular()
		{
			return specular;
		}
		float GetConstant()
		{
			return constant;
		}
		float GetLinear()
		{
			return linear;
		}
		float GetQuadratic()
		{
			return quadratic;
		}

		void SetAmbient(glm::vec3 Ambient)
		{
			ambient = Ambient;
		}

		void SetDiffuse(glm::vec3 Diffuse)
		{
			diffuse = Diffuse;
		}
		void SetSpecular(glm::vec3 Specular)
		{
			specular = Specular;
		}
		void SetConstant(float Constant)
		{
			constant = Constant;
		}
		void SetLinear(float Linear)
		{
			linear = Linear;
		}
		void SetQuadratic(float Quadratic)
		{
			quadratic = Quadratic;
		}


	};

}

#endif