#include "FrameNode.h"
#include "InputSocket.h"
#include "OutputSocket.h"

Fracture::FrameNode::FrameNode(const std::string& name):m_name(name)
{
	isVisited = false;
}

void Fracture::FrameNode::AddInputSocket(std::shared_ptr<InputSocket> input)
{
	for (auto& socket : m_inputs)
	{
		if (socket->GetName() == input->GetName())
		{
			FRACTURE_ERROR("Socket - {} - in FrameNode already exists!", socket->GetName());
			return;
		}
	}
	m_inputs.push_back(input);
}

void Fracture::FrameNode::AddOutputSocket(std::shared_ptr<OutputSocket> output)
{
	for (auto& socket : m_outputs)
	{
		if (socket->GetName() == output->GetName())
		{
			FRACTURE_ERROR("Socket - {} - in FrameNode already exists!", socket->GetName());
			return;
		}
	}
	m_outputs.push_back(output);
}

void Fracture::FrameNode::AddInputResource(std::shared_ptr<InputSocket> input,std::shared_ptr<FrameResource> resource)
{
	resources[input->GetName()] = resource;
}

void Fracture::FrameNode::AddOutputResource(std::shared_ptr<OutputSocket> output, std::shared_ptr<FrameResource> resource)
{
	resources[output->GetName()] = resource;
}

void Fracture::FrameNode::AddResource(std::string name, std::shared_ptr<FrameResource> res)
{
	resources[name] = res;
}

std::shared_ptr<Fracture::FrameResource> Fracture::FrameNode::getResource(std::string name)
{
	return resources[name];
}

void Fracture::FrameNode::LinkResource(std::string name, std::shared_ptr<FrameResource> res)
{
	std::shared_ptr<FrameResource> registered_res = resources[name];
	registered_res = res;
	resources[name] = registered_res;
}

std::string Fracture::FrameNode::GetName()
{
	return m_name;
}

void Fracture::FrameNode::Resize(unsigned int width, unsigned int height)
{
	for (auto& res : resources)
	{
		if (res.second)
		{
			res.second->Resize(width, height);
		}		
	}
}
