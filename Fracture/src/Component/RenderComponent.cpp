#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "AssetManager/AssetManager.h"

Fracture::RenderComponent::RenderComponent(int entityID,std::string model):Component(entityID,ComponentType::Render),m_modelName(model)
{
}

Fracture::RenderComponent::~RenderComponent()
{
}

void Fracture::RenderComponent::onAttach()
{
    m_model = AssetManager::getModel(m_modelName);
}

void Fracture::RenderComponent::onDettach()
{
}

std::shared_ptr < Fracture:: Model > Fracture::RenderComponent::GetModel()
{
    return m_model;
}
