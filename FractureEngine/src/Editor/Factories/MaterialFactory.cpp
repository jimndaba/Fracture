#include "MaterialFactory.h"
#include "AssetManager/AssetManager.h"

Fracture::MaterialFactory::MaterialFactory()
{
}

Fracture::MaterialFactory::~MaterialFactory()
{
}

std::shared_ptr <Fracture::Material > Fracture::MaterialFactory::OpaqueMaterial()
{
	return std::shared_ptr<Material>();
}

std::shared_ptr<Fracture::Material> Fracture::MaterialFactory::PBRMaterial(const std::string& name)
{
    std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(name,AssetManager::getShader("PBRTexturedShader")));

     material->setFloat("albedoFlag", 0.0f);
     material->setFloat("TransparencyFlag", 0.0f);
     material->setColor3("u_albedo", glm::vec3(0.9f, 0.9f, 0.9f));

     material->setFloat("normalFlag",0.0f);

     material->setFloat("metallicFlag", 0.0f);
     material->setFloat("u_metallic", 0.0f);

     material->setFloat("roughnessFlag", 0.0f);
     material->setFloat("u_roughness", 0.0f);

     material->setFloat("aoFlag", 0.0f);
     material->setFloat("u_ao", 0.1f);

	return material;
}
