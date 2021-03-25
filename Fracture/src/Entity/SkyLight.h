#pragma once
#ifndef SKYLIGHT_H
#define SKYLIGHT_H


#include "ILight.h"

namespace Fracture
{
	class StaticEnvironment;
	class Texture2D;
	class TextureCubeMap;

	class SkyLight : public ILight
	{

	public:
		SkyLight(const std::string& name);
		SkyLight(const std::string& name,const std::shared_ptr<Texture2D>& hdr);
		~SkyLight() = default;
		virtual void SetAmbient(glm::vec4 ambient) { m_ambient = ambient; };
		virtual void SetDiffuse(glm::vec4 diffuse) { m_diffuse = diffuse; };
		virtual void SetSpecular(glm::vec4 specular) { m_specular = specular; };
		virtual glm::vec4 GetAmbient() { return m_ambient; };
		virtual glm::vec4 GetDiffuse() { return m_diffuse; };
		virtual glm::vec4 GetSpecular() { return m_specular; };
		
		
		virtual LightType GetLightType();
		
		std::shared_ptr<TextureCubeMap> GetIrradianceMap();

		std::shared_ptr<TextureCubeMap> GetPreFilterMap();

		std::shared_ptr<Texture2D> GetBDRFMap();

		std::shared_ptr<StaticEnvironment> GetEnvironment()const;	

		void ChangeEnvironment(const std::string& name);		

	private:
		std::shared_ptr<StaticEnvironment> m_environment;
		glm::vec4 m_ambient = glm::vec4(1.0f);
		glm::vec4 m_diffuse = glm::vec4(0.6f);
		glm::vec4 m_specular = glm::vec4(0.6f);
	};


}

#endif 