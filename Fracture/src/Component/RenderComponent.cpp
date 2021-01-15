#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/RenderCommand.h"

Fracture::RenderComponent::RenderComponent(uint32_t entityID, std::shared_ptr<Model> model):Component(entityID,ComponentType::Render),m_model(model)
{
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
	m_material = AssetManager::getMaterial(name);
}

std::shared_ptr<Fracture::Material> Fracture::RenderComponent::GetMaterial()
{
	return m_material;
}

void Fracture::RenderComponent::SetModel(std::string name)
{
	m_model = AssetManager::getModel(name);
}

void Fracture::RenderComponent::SetRenderType(RenderType m_type)
{
	m_Rendertype = m_type;
}

Fracture::RenderType Fracture::RenderComponent::getRenderType()
{
	return m_Rendertype;
}

void Fracture::RenderComponent::Accept(ISceneProbe* visitor)
{
	visitor->VisitRenderComponent(this);
}
