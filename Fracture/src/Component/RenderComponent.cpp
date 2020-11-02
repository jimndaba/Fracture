#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/RenderCommand.h"

Fracture::RenderComponent::RenderComponent(uint32_t entityID,std::string modelname,std::string materialname):Component(entityID,ComponentType::Mesh),m_modelName(modelname),m_materialName(materialname)
{
	model = AssetManager::getModel(m_modelName);
	material = AssetManager::getMaterial(m_materialName);
	m_Rendertype = RenderType::Normal;
}

Fracture::RenderComponent::~RenderComponent()
{
}

void Fracture::RenderComponent::onStart()
{
}

void Fracture::RenderComponent::SetMaterial(std::string name)
{
	material = AssetManager::getMaterial(name);
}

void Fracture::RenderComponent::SetModel(std::string name)
{
	model = AssetManager::getModel(name);
}

void Fracture::RenderComponent::SetRenderType(RenderType m_type)
{
	m_Rendertype = m_type;
}

Fracture::RenderType Fracture::RenderComponent::getRenderType()
{
	return m_Rendertype;
}
