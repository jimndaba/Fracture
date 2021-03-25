#include "DynamicSkylight.h"
#include "Rendering/DynamicEnvironment.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/TextureCubeMap.h"
#include "Rendering/Material.h"

Fracture::DynamicSkyLight::DynamicSkyLight(const std::string& name):
	ILight(),
	m_environment(DynamicEnvironment::Create(name, AssetManager::getShader("ProcedualSkybox")))
{
    m_material = std::make_shared<Material>("DynamicSky", m_environment->GetShader());
}


Fracture::LightType Fracture::DynamicSkyLight::GetLightType()
{
    return LightType::DynamicSky;
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::DynamicSkyLight::GetIrradianceMap()
{
    return m_environment->m_IrradianceMap;
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::DynamicSkyLight::GetPreFilterMap()
{
    return m_environment->m_PrefilterMap;
}

std::shared_ptr<Fracture::Texture2D> Fracture::DynamicSkyLight::GetBDRFMap()
{
    return m_environment->m_bdrfTexture;
}

std::shared_ptr<Fracture::DynamicEnvironment> Fracture::DynamicSkyLight::GetEnvironment() const
{
    return m_environment;
}

void Fracture::DynamicSkyLight::ChangeEnvironment(const std::string& name)
{
	
}

std::shared_ptr<Fracture::Material> Fracture::DynamicSkyLight::GetMaterial()
{
	return m_material;
}
