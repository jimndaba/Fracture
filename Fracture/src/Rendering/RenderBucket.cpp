#include "RenderBucket.h"
#include "RenderCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Logging/Logger.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

bool renderSortforward(const Fracture::RenderCommand& a, const Fracture::RenderCommand& b);

Fracture::RenderBucket::RenderBucket()
{
}

Fracture::RenderBucket::~RenderBucket()
{
}

void Fracture::RenderBucket::pushCommand(RenderCommand command)
{
	m_commands.push_back(command);
}

void Fracture::RenderBucket::pushCommand(std::shared_ptr<Fracture::Mesh> mesh,std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
	RenderCommand command = RenderCommand(material.get());
	
	for (int i = 0; i < mesh->Textures().size(); i++)
	{
		//command.material->SetTexture(mesh->Textures()[i]->type, mesh->Textures()[i], (int)mesh->Textures()[i]->textureType);
		command.TextureNames.push_back(mesh->Textures()[i]->Name);
		std::string sampleType;
		switch (mesh->Textures()[i]->textureType)
		{
		case TextureType::Diffuse:
		{
			sampleType = "material.diffuse";
			break;
		}
		case TextureType::Specular:
		{
			sampleType = "material.specular";
			break;
		}
		case TextureType::Normal:
		{
			sampleType = "material.normal";
			break;
		}
		case TextureType::Height:
		{
			sampleType = "material.height";
			break;
		}
		case TextureType::Bump:
		{
			sampleType = "material.bump";
			break;
		}
		case TextureType::Reflection:
		{
			sampleType = "material.reflection";
			break;
		}
		case TextureType::ColorAttachment:
		{
			sampleType = "ColorAttachment";
			break;
		}
		case TextureType::DepthStencilAttachment:
		{
			sampleType = "DepthStencilAttachment";
			break;
		}
		}

		material->SetTexture(sampleType, mesh->Textures()[i], (int)mesh->Textures()[i]->textureType);
	}


	command.VAO = mesh->VAO;
	command.material = material.get();
	command.HasTransparency = material.get()->IsTransparent();
	command.indiceSize = (GLint)mesh->GetIndices().size();
	command.ID= transform->EntityID;
	m_commands.push_back(command);
}

void Fracture::RenderBucket::sort()
{
	std::sort(m_commands.begin(), m_commands.end(), renderSortforward);
}

void Fracture::RenderBucket::clear()
{
	m_commands.clear();
}

std::vector<Fracture::RenderCommand> Fracture::RenderBucket::getCommands(bool cull)
{
	return m_commands;
}

bool renderSortforward(const Fracture::RenderCommand& a, const Fracture::RenderCommand& b)
{
	return a < b;
}


