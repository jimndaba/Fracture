#include "RenderBucket.h"
#include "RenderCommand.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "RenderBatch.h"
#include "Logging/Logger.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Profiling/Profiler.h"

bool renderSortforward(const Fracture::RenderCommand& a, const Fracture::RenderCommand& b);

Fracture::RenderBucket::RenderBucket()
{
}

Fracture::RenderBucket::~RenderBucket()
{
}

void Fracture::RenderBucket::pushCommand(RenderCommand command)
{
	std::map<std::string, std::shared_ptr<RenderBatch>>::iterator it = m_batches.find(command.material->Name);
	if (it != m_batches.end())
	{
		m_batches[command.material->Name]->m_commnads.push_back(command);
	}
	else
	{
		std::shared_ptr<RenderBatch> batch = std::make_shared<RenderBatch>();
		m_batches[command.material->Name] = batch;
		batch->m_commnads.push_back(command);
	}
}

void Fracture::RenderBucket::pushCommand(std::shared_ptr<Fracture::Mesh> mesh,std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
	ProfilerTimer timer("Create Command");
	RenderCommand command = RenderCommand{};	
	command.VAO = mesh->VAO;
	command.material = material.get();
	command.HasTransparency = material.get()->IsTransparent();
	command.IsOutlined = material->IsOutlined();
	command.ID = transform->EntityID;
	command.indiceSize = (GLint)mesh->IndexCount;	
		
	std::map<std::string, std::shared_ptr<RenderBatch>>::iterator it = m_batches.find(material->Name);	
	if (it != m_batches.end())
	{		
		m_batches[material->Name]->m_commnads.push_back(command);
	}
	else
	{			
		std::shared_ptr<RenderBatch> batch = std::make_shared<RenderBatch>();
		m_batches[material->Name] = batch;
		batch->m_commnads.push_back(command);
	}

}

void Fracture::RenderBucket::sort()
{
	for (auto batches : m_batches)
	{
		std::sort(batches.second->m_commnads.begin(), batches.second->m_commnads.end(),renderSortforward);
	}
}

void Fracture::RenderBucket::clear()
{
	for (auto batches : m_batches)
	{
		batches.second->m_commnads.clear();
	}
	m_batches.clear();
}

std::vector<Fracture::RenderCommand> Fracture::RenderBucket::getCommands(bool cull)
{
	
	return m_commands;
}

std::map<std::string, std::shared_ptr<Fracture::RenderBatch>> Fracture::RenderBucket::getRenderBatches()
{
	return m_batches;
}

bool renderSortforward(const Fracture::RenderCommand& a, const Fracture::RenderCommand& b)
{
	return a < b;
}


