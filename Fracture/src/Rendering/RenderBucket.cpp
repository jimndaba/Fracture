#include "RenderBucket.h"
#include "RenderCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
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
	RenderCommand command = {};
	
	command.material = material;
	command.VAO = mesh->VAO;
	command.indiceSize = (GLint)mesh->GetIndices().size();
	command.transform = transform;

	command.texture_id = mate;
	command.texture_unit;

	/*
	for (int i = 0; i < mesh->Textures().size(); i++)
	{
		if (mesh->Textures()[i]->textureType == TextureType::Diffuse)
		{
			command.material->SetTexture("material.diffuse", mesh->Textures()[i],0);
		}
		if (mesh->Textures()[i]->textureType == TextureType::Normal)
		{
			command.material->SetTexture("material.normal", mesh->Textures()[i],1);
		}
		if (mesh->Textures()[i]->textureType == TextureType::Specular)
		{
			command.material->SetTexture("material.specular", mesh->Textures()[i], 2);
		}
		if (mesh->Textures()[i]->textureType == TextureType::Reflection)
		{
			command.material->SetTexture("material.reflection", mesh->Textures()[i],3);
		}
		if (mesh->Textures()[i]->textureType == TextureType::Height)
		{
			command.material->SetTexture("material.height", mesh->Textures()[i], 4);
		}
		if (mesh->Textures()[i]->textureType == TextureType::Bump)
		{
			command.material->SetTexture("material.bump", mesh->Textures()[i], 5);
		}		
	}
	*/

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
	return a.material->getShader()->ID() < b.material->getShader()->ID();
}
