#include "RenderBucket.h"
#include "DrawCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Logging/Logger.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"
#include "Shader.h"
#include "Material.h"
#include "OpenGL/Mesh.h"
#include "Profiling/Profiler.h"

bool renderSortforward(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b);
bool renderSortAlpha(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b);

Fracture::RenderBucket::RenderBucket()
{
}

Fracture::RenderBucket::~RenderBucket()
{
}

void Fracture::RenderBucket::pushCommand(const DrawCommand& command)
{	
	if (command.HasTransparency)
	{
		m_Alphacommands.push_back(command);
	}
	else
	{
		m_Frowardcommands.push_back(command);
	}

	if (command.CastShadows)
	{
		m_Shadowcommands.push_back(command);
	}

	if (command.IsOutlined)
	{
		//m_Outlinecommands.push_back(command);
	}

}

void Fracture::RenderBucket::pushCommand(const UUID& EntityID, glm::vec4 color, std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, glm::mat4 transform)
{
	ProfilerTimer timer("Create Command");
	DrawCommand command = DrawCommand{};	
	command.VAO = mesh->RenderID();
	command.material = material.get();
	command.CastShadows = material->CastShadows();
	command.HasTransparency = material->IsTransparent();
	command.IsOutlined = material->IsOutlined();
	command.ID = EntityID;
	command.indiceSize = mesh->CountOfIndices();	
	command.Transform = transform;
	command.Color = color;
	pushCommand(command);
}

void Fracture::RenderBucket::pushOutlineCommand(const UUID& EntityID, std::shared_ptr<Fracture::Mesh> mesh, glm::mat4 transform)
{
	DrawCommand command = DrawCommand{};
	command.VAO = mesh->RenderID();
	command.CastShadows = false;
	command.HasTransparency =false;
	command.IsOutlined = true;
	command.ID = EntityID;
	command.indiceSize = mesh->CountOfIndices();
	command.Transform = transform;
	m_Outlinecommands.push_back(command);
}

void Fracture::RenderBucket::sortForward()
{
	std::sort(m_Frowardcommands.begin(), m_Frowardcommands.end(), renderSortforward);
}

void Fracture::RenderBucket::sortAlpha()
{
	std::sort(m_Alphacommands.begin(), m_Alphacommands.end(), renderSortAlpha);
}

void Fracture::RenderBucket::clear()
{	
	m_Alphacommands.clear();
	m_Frowardcommands.clear();
	m_Shadowcommands.clear();
	m_Outlinecommands.clear();
}

std::vector<Fracture::DrawCommand> Fracture::RenderBucket::getForwardRenderCommands()
{
	return m_Frowardcommands;
}

std::vector<Fracture::DrawCommand> Fracture::RenderBucket::getAlphaRenderCommands()
{
	return m_Alphacommands;
}

std::vector<Fracture::DrawCommand> Fracture::RenderBucket::getShadowRenderCommands()
{
	return m_Shadowcommands;
}

std::vector<Fracture::DrawCommand> Fracture::RenderBucket::getOutlineRenderCommands()
{
	return m_Outlinecommands;
}

bool renderSortforward(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b)
{
	return a < b;
}

bool renderSortAlpha(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b)
{
	return a > b;
}



