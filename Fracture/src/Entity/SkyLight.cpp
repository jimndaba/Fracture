#include "SkyLight.h"
#include "Rendering/Environment.h"
#include "Rendering/Texture.h"

Fracture::SkyLight::SkyLight():
	ILight(),
	m_environment(std::make_shared<Environment>(AssetManager::getTexture("Loft"), AssetManager::getShader("CubeMap")))
{
	
}

Fracture::LightType  Fracture::SkyLight::GetLightType()
{
	return LightType::Sky;
}

unsigned int  Fracture::SkyLight::GetIrradianceMap()
{
	return m_environment->irradianceMap;
}

unsigned int  Fracture::SkyLight::GetPreFilterMap()
{
	return m_environment->prefilterMap;
}

std::shared_ptr<Fracture::Texture>  Fracture::SkyLight::GetBDRFMap()
{
	return m_environment->m_bdrfTexture;
}

std::shared_ptr<Fracture::Environment> Fracture::SkyLight::GetEnvironment()
{
	return m_environment;
}

void Fracture::SkyLight::ChangeEnvironment(const std::string& name)
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