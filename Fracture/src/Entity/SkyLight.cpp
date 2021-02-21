#include "SkyLight.h"
#include "Rendering/Environment.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/TextureCubeMap.h"

Fracture::SkyLight::SkyLight():
	ILight(),
	m_environment(Environment::Create(std::static_pointer_cast<Texture2D>(AssetManager::getTexture("Loft")), AssetManager::getShader("CubeMap")))
{
	
}

Fracture::LightType  Fracture::SkyLight::GetLightType()
{
	return LightType::Sky;
}

std::shared_ptr<Fracture::TextureCubeMap>  Fracture::SkyLight::GetIrradianceMap()
{
	return m_environment->m_IrradianceMap;
}

std::shared_ptr<Fracture::TextureCubeMap>  Fracture::SkyLight::GetPreFilterMap()
{
	return m_environment->m_PrefilterMap;
}

std::shared_ptr<Fracture::Texture2D>  Fracture::SkyLight::GetBDRFMap()
{
	return m_environment->m_bdrfTexture;
}

std::shared_ptr<Fracture::Environment> Fracture::SkyLight::GetEnvironment()
{
	return m_environment;
}

void Fracture::SkyLight::ChangeEnvironment(const std::string& name)
{
	std::shared_ptr<Texture2D> texture = std::static_pointer_cast<Texture2D>(AssetManager::getTexture(name));
	std::shared_ptr<Environment> newEnvironment = Environment::Create(texture, AssetManager::getShader("CubeMap"));
		
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