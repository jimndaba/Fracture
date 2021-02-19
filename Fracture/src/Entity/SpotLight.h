#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "ILight.h"

namespace Fracture {

    class SpotLight :
        public ILight
    {
	public:
		SpotLight() :ILight()
		{}

		virtual void SetAmbient(glm::vec4 ambient) { m_ambient = ambient; };
		virtual void SetDiffuse(glm::vec4 diffuse) { m_diffuse = diffuse; };
		virtual void SetSpecular(glm::vec4 specular) { m_specular = specular; };
		virtual void SetPosition(glm::vec3 position) { m_position = position; };
		virtual void SetDirection(glm::vec3 direction) { m_rotation = direction; };
		virtual void SetConstant(float constant) { m_constant = constant; };
		virtual void SetLinear(float linear) { m_linear = linear; };
		virtual void SetQuadratic(float quad) { m_quadratic = quad; };
		virtual void SetCutoff(float Coff) { m_cutoff = Coff; };
		virtual void SetOuterCutOff(float outerCoff) { m_outerCutOff = outerCoff; };

		virtual glm::vec4 GetAmbient() { return m_ambient; };
		virtual glm::vec4 GetDiffuse() { return m_diffuse; };
		virtual glm::vec4 GetSpecular() { return m_specular; };
		virtual glm::vec3 GetPosition() { return m_position; };
		virtual glm::vec3 GetDirection() { return m_rotation; };
		virtual float GetConstant() { return m_constant; };
		virtual float GetLinear() { return m_linear; };
		virtual float GetQuadratic() { return m_quadratic; };
		virtual float GetCutoff() { return m_cutoff; };
		virtual float GetOuterCutOff() { return m_outerCutOff; };

		virtual LightType GetLightType()
		{
			return LightType::Spot;
		}

	private:
		glm::vec4 m_ambient = glm::vec4(1.0f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(0.6f);
		glm::vec3 m_position = glm::vec3(0.0f, 5.0f, 0.0);
		glm::vec3 m_rotation = glm::vec3(0.0f, -1.0f, 0.0f);
		float m_cutoff = 0.91f;
		float m_outerCutOff = 0.82f;
		float m_constant = 1.0f;
		float m_linear = 0.09f;
		float m_quadratic = 0.032f;
    };

}
#endif
