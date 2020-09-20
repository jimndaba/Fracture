#include "RenderBucket.h"
#include "RenderCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Mesh.h"
#include "Shader.h"

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

void Fracture::RenderBucket::pushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::TransformComponent> transform)
{
	RenderCommand command;
	

	command.VAO = mesh->VAO;
	command.IAO = mesh->IBO;
	command.VBO = mesh->VBO;

	command.transform = transform;

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
