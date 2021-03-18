#include "UINode.h"
#include "Logging/Logger.h"
#include "UISocket.h"
#include "UIResource/UIResource.h"

Fracture::UINode::UINode(const std::string& name):m_name(name)
{
}

void Fracture::UINode::AddInputSocket(UISocket input)
{
	for (auto& socket : m_inputs)
	{
		if (socket.GetName() == input.GetName())
		{
			FRACTURE_ERROR("Socket - {} - in FrameNode already exists!", socket.GetName());
			return;
		}
	}
	m_inputs.push_back(input);
}

void Fracture::UINode::AddMultiInputSocket(UIMultiSocket input)
{
	for (auto& socket : m_MultiInputs)
	{
		if (socket.GetName() == input.GetName())
		{
			FRACTURE_ERROR("Socket - {} - in FrameNode already exists!", socket.GetName());
			return;
		}
	}
	m_MultiInputs.push_back(input);
}

void Fracture::UINode::AddOutputSocket(UISocket output)
{
	for (auto& socket : m_outputs)
	{
		if (socket.GetName() == output.GetName())
		{
			FRACTURE_ERROR("Socket - {} - in FrameNode already exists!", socket.GetName());
			return;
		}
	}
	m_outputs.push_back(output);
}

void Fracture::UINode::AddInputResource(UISocket input, std::shared_ptr<UIResource> resource)
{
	resources[input.GetName()] = resource;
}

void Fracture::UINode::AddMultiInputResource(UISocket input, std::shared_ptr<UIResource> resource)
{
	resources[input.GetName()] = resource;
}

void Fracture::UINode::AddOutputResource(UISocket output, std::shared_ptr<UIResource> resource)
{
	resources[output.GetName()] = resource;
}

void Fracture::UINode::AddResource(std::string name, std::shared_ptr<UIResource> res)
{
	resources[name] = res;
}

std::shared_ptr<Fracture::UIResource> Fracture::UINode::getResource(std::string name)
{
    return resources[name];
}

void Fracture::UINode::LinkUIResource(std::string name, std::shared_ptr<UIResource> res)
{	
	resources[name] = res;
}

std::string Fracture::UINode::GetName() const
{
    return m_name;
}
