#pragma once
#ifndef ILIGHTCOMPONENT_H
#define ILIGHTCOMPONENT_H

#include "glm/glm.hpp"
#include <memory>
#include <string>
#include "AssetManager/AssetManager.h"
#include "Rendering/Environment.h"
#include "Rendering/Texture.h"
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
		virtual glm::vec3 GetDirection() { return glm::vec3(0); };
		virtual float GetConstant() { return 0.0f; };
		virtual float GetLinear() { return 0.0f; };
		virtual float GetQuadratic() { return 0.0f; };
		virtual float GetCutoff() { return 0.0f; };
		virtual float GetOuterCutOff() { return 0.0f; };
		virtual float GetRadiance() { return 0.0f; };
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
		float m_intensity = 100.0f;
	};

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
			m_direction = glm::vec3(0.0f,-1.0f,0.0f);
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
		//virtual void SetPosition(glm::vec3 position) {};
		virtual void SetDirection(glm::vec3 direction)
		{
			m_direction = direction;
		};

		void SetRadiance(float value)
		{
			radiance = value;
		}
		//virtual void SetConstant(float constant) {};
		//virtual void SetLinear(float linear) {};
		//virtual void SetQuadratic(float quad) {};
		//virtual void SetCutoff(float Coff) {};
		//virtual void SetOuterCutOff(float outerCoff) {};

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
		//virtual glm::vec3 GetPosition() {};
		virtual glm::vec3 GetDirection()
		{
			return m_direction;
		};

		virtual float GetRadiance()
		{
			return radiance;
		}
		//virtual float GetConstant() {};
		//virtual float GetLinear() {};
		//virtual float GetQuadratic() {};
		//virtual float GetCutoff() {};
		//virtual float GetOuterCutOff() {};
		virtual LightType GetLightType()
		{
			return LightType::Sun;
		}

	private:		
		glm::vec4 m_ambient = glm::vec4(0.6f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(1.0f);
		glm::vec3 m_direction = glm::vec3(0.0f,-1.0f,0.0f);
		float radiance = 1.0f;
	};

	class PointLight : public ILight
	{
	public:
		PointLight() :ILight()
		{
		}

		virtual void SetAmbient(glm::vec4 ambient) { m_ambient = ambient; };
		virtual void SetDiffuse(glm::vec4 diffuse) { m_diffuse = diffuse; };
		virtual void SetSpecular(glm::vec4 specular) { m_specular = specular; };
		virtual void SetPosition(glm::vec3 position) { m_position = position; };
		virtual void SetDirection(glm::vec3 direction) { m_rotation = direction; };
		virtual void SetConstant(float constant) { m_constant = constant; };
		virtual void SetLinear(float linear) { m_linear = linear; };
		virtual void SetQuadratic(float quad) { m_quadratic = quad; };
		

		virtual glm::vec4 GetAmbient() { return m_ambient; };
		virtual glm::vec4 GetDiffuse() { return m_diffuse; };
		virtual glm::vec4 GetSpecular() { return m_specular; };
		virtual glm::vec3 GetPosition() { return m_position; };
		virtual glm::vec3 GetDirection() { return m_rotation; };
		virtual float GetConstant() { return m_constant; };
		virtual float GetLinear() { return m_linear; };
		virtual float GetQuadratic() { return m_quadratic; };
		
		virtual LightType GetLightType()
		{
			return LightType::Point;
		}

	private:

		glm::vec4 m_ambient = glm::vec4(1.0f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(0.6f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);;
		float m_constant = 1.0f;
		float m_linear = 0.09f;
		float m_quadratic = 0.032f;
		

	};

	class SpotLight:public ILight
	{
	public:
		SpotLight():ILight()
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
		glm::vec3 m_rotation = glm::vec3(0.0f,-1.0f,0.0f);
		float m_cutoff = 0.91f;
		float m_outerCutOff = 0.82f;
		float m_constant = 1.0f;
		float m_linear = 0.09f;
		float m_quadratic = 0.032f;
	};

	class SkyLight: public ILight
	{

	public:
		SkyLight() :ILight() {
		
			m_environment = std::shared_ptr<Environment>(new Environment(AssetManager::getTexture("Loft"), AssetManager::getShader("CubeMap")));
		
		}
		~SkyLight() {}
		virtual void SetAmbient(glm::vec4 ambient) { m_ambient = ambient; };
		virtual void SetDiffuse(glm::vec4 diffuse) { m_diffuse = diffuse; };
		virtual void SetSpecular(glm::vec4 specular) { m_specular = specular; };
		virtual glm::vec4 GetAmbient() { return m_ambient; };
		virtual glm::vec4 GetDiffuse() { return m_diffuse; };
		virtual glm::vec4 GetSpecular() { return m_specular; };
		virtual LightType GetLightType()
		{
			return LightType::Sky;
		}

		unsigned int GetIrradianceMap()
		{
			return m_environment->irradianceMap;
		}
		unsigned int GetPreFilterMap()
		{
			return m_environment->prefilterMap;
		}
		std::shared_ptr<Texture> GetBDRFMap()
		{
			return m_environment->m_bdrfTexture;
		}

		std::shared_ptr<Environment> GetEnvironment()
		{
			return m_environment;
		}
		void ChangeEnvironment(const std::string& name)
		{
			std::shared_ptr<Environment> newEnvironment = std::shared_ptr<Environment>(new Environment(AssetManager::getTexture(name), AssetManager::getShader("CubeMap")));
			if (newEnvironment)
			{
				m_environment.reset();
				m_environment = newEnvironment;
			}
			else
			{
				FRACTURE_ERROR("FAILED TO LOAD ENVIRONMENT: {}", name);
			}			
		}

	private:
		std::shared_ptr<Environment> m_environment;
		glm::vec4 m_ambient = glm::vec4(1.0f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(0.6f);
	};

}

#endif