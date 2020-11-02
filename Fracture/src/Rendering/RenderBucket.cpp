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
	command.material = material.get();

	for (int i = 0; i < mesh->Textures().size(); i++)
	{
		command.Textures.push_back(mesh->Textures()[i]);
	}

	command.VAO = mesh->VAO;
	command.HasTransparency = 0;
	command.indiceSize = (GLint)mesh->GetIndices().size();
	command.ID= transform->EntityID;
	m_commands.push_back(command);
}

void Fracture::RenderBucket::pushInstancedElementCommand(std::shared_ptr<RenderInstancedElementsCommand> command)
{
	m_InstancedElemtcommands.push_back(command);
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

std::vector<std::shared_ptr<Fracture::RenderInstancedElementsCommand>> Fracture::RenderBucket::GetInstanced(bool cull)
{
	return m_InstancedElemtcommands;
}

bool renderSortforward(const Fracture::RenderCommand& a, const Fracture::RenderCommand& b)
{
	return a < b;
}
