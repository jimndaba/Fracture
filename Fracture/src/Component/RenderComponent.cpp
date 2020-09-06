#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "AssetManager/AssetManager.h"

Fracture::RenderComponent::RenderComponent(int entityID,std::string model,std::string material):Component(entityID,ComponentType::Render),m_modelName(model),m_materialName(material)
{
}

Fracture::RenderComponent::~RenderComponent()
{
}

void Fracture::RenderComponent::onAttach()
{
    model = AssetManager::getModel(m_modelName);
    material = AssetManager::getMaterial(m_materialName);
}

void Fracture::RenderComponent::onDettach()
{
}
