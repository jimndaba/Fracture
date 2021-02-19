#pragma once
#ifndef ILIGHTCOMPONENT_H
#define ILIGHTCOMPONENT_H

#include "glm/glm.hpp"
#include <memory>
#include <string>
#include "AssetManager/AssetManager.h"
#include "Logging/Logger.h"

namespace Fracture
{
	enum class LightType
	{
		Sun,
		Point,
		Spot,
		Sky
	};

	class ILight
	{

	public:
		ILight()
		{};
		~ILight() {};
		virtual void SetAmbient(glm::vec4 ambient) = 0;
		virtual void SetDiffuse(glm::vec4 diffuse) = 0;
		virtual void SetSpecular(glm::vec4 specular) = 0;
		virtual void SetPosition(glm::vec3 position) {};
		virtual void SetDirection(glm::vec3 direction) {};
		virtual void SetConstant(float constant) {};
		virtual void SetLinear(float linear) {};
		virtual void SetQuadratic(float quad) {};
		virtual void SetCutoff(float Coff) {};
		virtual void SetOuterCutOff(float outerCoff) {};
		void SetIntensity(float value);

		virtual glm::vec4 GetAmbient() = 0;
		virtual glm::vec4 GetDiffuse() = 0;
		virtual glm::vec4 GetSpecular() = 0;
		virtual glm::vec3 GetPosition() { return glm::vec3(0); };
		virtual glm::vec3 GetDirection() { return glm::vec3(0.0f,-1.0f,0.0f); };
		virtual float GetConstant() { return 0.0f; };
		virtual float GetLinear() { return 0.0f; };
		virtual float GetQuadratic() { return 0.0f; };
		virtual float GetCutoff() { return 0.0f; };
		virtual float GetOuterCutOff() { return 0.0f; };
		virtual glm::vec3 GetRadiance() { return glm::vec3(0.0f); };
		float Intensity();
	

		virtual LightType GetLightType() = 0;
		void SetCastShadows(bool value)
		{
			m_castShadows = value;
		}
		bool CastShadows()
		{
			return m_castShadows;
		}

	private:
		bool m_castShadows = true;
		float m_intensity = 1.0f;
	};
}

#endif