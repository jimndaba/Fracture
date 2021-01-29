#include "RenderBucket.h"
#include "DrawCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "RenderBatch.h"
#include "Logging/Logger.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Profiling/Profiler.h"

bool renderSortforward(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b);

Fracture::RenderBucket::RenderBucket()
{
	mType = BucketType::Opaque;
}

Fracture::RenderBucket::~RenderBucket()
{
}

void Fracture::RenderBucket::pushCommand(DrawCommand command)
{
	//std::map<std::string, std::shared_ptr<RenderBatch>>::iterator it = m_batches.find(command.material->Name);
	//if (it != m_batches.end())
	//{
	//	m_batches[command.material->Name]->m_commnads.push_back(command);
	//}
	//else
	//{
	//	std::shared_ptr<RenderBatch> batch = std::make_shared<RenderBatch>();
	//	m_batches[command.material->Name] = batch;
	//	batch->m_commnads.push_back(command);
	//}

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

}

void Fracture::RenderBucket::pushCommand(uint32_t EntityID, std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, glm::mat4 transform)
{
	ProfilerTimer timer("Create Command");
	DrawCommand command = DrawCommand{};	
	command.VAO = mesh->VAO;
	command.material = material.get();
	command.CastShadows = material->CastShadows();
	command.HasTransparency = material.get()->IsTransparent();
	command.IsOutlined = material->IsOutlined();
	command.ID = EntityID;
	command.indiceSize = (GLint)mesh->IndexCount;	
	command.Transform = transform;

	if (command.HasTransparency)
	{
		m_Alphacommands.push_back(std::move(command));
	}
	else
	{
		m_Frowardcommands.push_back(std::move(command));
	}

	if (command.CastShadows)
	{
		m_Shadowcommands.push_back(std::move(command));
	}

	//std::map<std::string, std::shared_ptr<RenderBatch>>::iterator it = m_batches.find(material->Name);	
	//if (it != m_batches.end())
	//{		
	//	m_batches[material->Name]->m_commnads.push_back(command);
	//}
	//else
	//{			
	//	std::shared_ptr<RenderBatch> batch = std::make_shared<RenderBatch>();
	//	m_batches[material->Name] = batch;
	//	batch->m_commnads.push_back(command);
	//}

}

void Fracture::RenderBucket::sort()
{
	//for (auto batches : m_batches)
	//{
	//	std::sort(batches.second->m_commnads.begin(), batches.second->m_commnads.end(),renderSortforward);
	//}

	std::sort(m_Frowardcommands.begin(), m_Frowardcommands.end(), renderSortforward);
}

void Fracture::RenderBucket::clear()
{
	//for (auto batches : m_batches)
	//{
	//	batches.second->m_commnads.clear();
	//}
	//m_batches.clear();
	m_Alphacommands.clear();
	m_Frowardcommands.clear();
	m_Shadowcommands.clear();
}

//std::vector<Fracture::DrawCommand> Fracture::RenderBucket::getCommands(bool cull)
//{
//	return m_commands;
//}

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

//std::map<std::string, std::shared_ptr<Fracture::RenderBatch>> Fracture::RenderBucket::getRenderBatches()
//{
//	return m_batches;
//}

bool renderSortforward(const Fracture::DrawCommand& a, const Fracture::DrawCommand& b)
{
	return a < b;
}


