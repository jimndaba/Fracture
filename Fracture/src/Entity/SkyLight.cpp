#include "SkyLight.h"
#include "Rendering/StaticEnvironment.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/TextureCubeMap.h"

Fracture::SkyLight::SkyLight(const std::string& name):
	ILight(),
	m_environment(StaticEnvironment::Create(name,std::static_pointer_cast<Texture2D>(AssetManager::getHDRTexture("Loft")), AssetManager::getShader("Skybox")))
{
	
}

Fracture::SkyLight::SkyLight(const std::string& name,const std::shared_ptr<Texture2D>& hdr):ILight(),
m_environment(StaticEnvironment::Create(name,hdr,AssetManager::getShader("Skybox")))
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

std::shared_ptr<Fracture::StaticEnvironment> Fracture::SkyLight::GetEnvironment()const
{
	return m_environment;
}

void Fracture::SkyLight::ChangeEnvironment(const std::string& name)
{
	std::shared_ptr<Texture2D> texture = AssetManager::getHDRTexture(name);
	std::shared_ptr<StaticEnvironment> newEnvironment = StaticEnvironment::Create("Skylight",texture, AssetManager::getShader("Skybox"));
		
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