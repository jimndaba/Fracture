#include "RenderComponent.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Model.h"
#include "Rendering/OpenGL/Mesh.h"
#include "Rendering/Material.h"

Fracture::RenderComponent::RenderComponent(UUID id,const std::shared_ptr<Model>& model):Component(id),
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

nlohmann::json Fracture::RenderComponent::serialise(const std::shared_ptr<ComponentSerialiser>& visitor)
{
	return visitor->visitRenderComponent(*this);
}
