#include "RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "AssetManager/AssetManager.h"

Fracture::RenderComponent::RenderComponent(int entityID,std::string model,std::string material):Component(entityID,ComponentType::Mesh),m_modelName(model),m_materialName(material)
{
    onAttach();
}

Fracture::RenderComponent::~RenderComponent()
{
}

void Fracture::RenderComponent::onAttach()
{
    model = AssetManager::getModel(m_modelName);
    material = AssetManager::getMaterial(m_materialName);
	/*
	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int y = 0; y < model->GetMeshes()[i]->Textures().size(); y++)
		{

			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Diffuse)
			{
				material->SetTexture("material.diffuse", model->GetMeshes()[i]->Textures()[y], 0);
			}
			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Normal)
			{
				material->SetTexture("material.normal", model->GetMeshes()[i]->Textures()[y], 1);
			}
			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Specular)
			{
				material->SetTexture("material.specular", model->GetMeshes()[i]->Textures()[y], 2);
			}
			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Reflection)
			{
				material->SetTexture("material.reflection", model->GetMeshes()[i]->Textures()[y], 3);
			}
			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Height)
			{
				material->SetTexture("material.height", model->GetMeshes()[i]->Textures()[y], 4);
			}
			if (model->GetMeshes()[i]->Textures()[y]->textureType == TextureType::Bump)
			{
				material->SetTexture("material.bump", model->GetMeshes()[i]->Textures()[y], 5);
			}
		}
	}
	*/
}

void Fracture::RenderComponent::onDettach()
{
}
