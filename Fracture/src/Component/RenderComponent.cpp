#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "AssetManager/AssetManager.h"

Fracture::RenderComponent::RenderComponent(uint32_t entityID,std::string modelname,std::string materialname):Component(entityID,ComponentType::Mesh),m_modelName(modelname),m_materialName(materialname)
{
	model = AssetManager::getModel(m_modelName);
	material = AssetManager::getMaterial(m_materialName);
}

Fracture::RenderComponent::~RenderComponent()
{
}

void Fracture::RenderComponent::onStart()
{
}
