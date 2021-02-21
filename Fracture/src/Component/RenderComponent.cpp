#include "RenderComponent.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Model.h"
#include "Rendering/OpenGL/Mesh.h"
#include "Rendering/Material.h"

Fracture::RenderComponent::RenderComponent(uint32_t entityID,const std::shared_ptr<Model>& model):Component(entityID,ComponentType::Render),
m_model(model)
{
}

void Fracture::RenderComponent::onStart()
{
}

void Fracture::RenderComponent::SetMaterial(const std::string& oldmaterial ,const std::string& name)
{
	m_model->SetMaterial(oldmaterial, AssetManager::getMaterial( name));
}

std::shared_ptr<Fracture::Material> Fracture::RenderComponent::GetMaterial()
{
	return m_material;
}

void Fracture::RenderComponent::SetModel(const std::string& name)
{
	m_model = AssetManager::getModel(name);
}

std::shared_ptr<Fracture::Model> Fracture::RenderComponent::GetModel() const
{
	return m_model;
}

void Fracture::RenderComponent::Accept(ISceneProbe* visitor)
{
	visitor->VisitRenderComponent(this);
}
