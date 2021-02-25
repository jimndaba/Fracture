#include "FrameGraphSerialiser.h"
#include "Rendering/Framegraph/PassLibrary/ClearFrame.h"
#include "Rendering/Framegraph/PassLibrary/LambertianNode.h"
#include "Rendering/Framegraph/PassLibrary/ToneMappingNode.h"
#include "Rendering/Framegraph/PassLibrary/ThresholdNode.h"
#include "Rendering/Framegraph/PassLibrary/AdditiveMixNode.h"
#include "Rendering/Framegraph/PassLibrary/GaussianBlurNode.h"
#include "Rendering/Framegraph/PassLibrary/DepthNode.h"
#include "Rendering/Framegraph/PassLibrary/SSAONode.h"
#include "Rendering/Framegraph/PassLibrary/MultiplyMix.h"
#include "Rendering/Framegraph/PassLibrary/PickingPass.h"
#include "Rendering/Framegraph/PassLibrary/IntermediateNode.h"
#include "Rendering/Framegraph/PassLibrary/OutlineNode.h"
#include "Rendering/Framegraph/PassLibrary/MixNode.h"
#include "Rendering/Framegraph/Link.h"
#include "Rendering/Framegraph/SourceNode.h"
#include "FrameGraphSerialisation/NodeSerialisers.h"
#include "Rendering/Framegraph/FrameGraph.h"

#include <typeinfo> 

enum class nodeType {
	ClearFrame,
	AdditiveMix,
	Mix,
	GaussianBlur,
	Depth,
	Intermediate,
	Lambertian,
	MultiplyMix,
	Outline,
	SSAO,
	Threshold,
	ToneMapping,
	Link,
	Sink,
	Source	
};

nodeType hashit(std::string const& inString) {
	if (inString == "class Fracture::ClearFrame") return nodeType::ClearFrame;
	if (inString == "class Fracture::AdditiveMixNode") return nodeType::AdditiveMix;
	if (inString == "class Fracture::GaussianBlurNode") return nodeType::GaussianBlur;
	if (inString == "class Fracture::DepthNode") return nodeType::Depth;
	if (inString == "class Fracture::IntermediateNode") return nodeType::Intermediate;
	if (inString == "class Fracture::LambertianNode") return nodeType::Lambertian;
	if (inString == "class Fracture::MultiplyMixNode") return nodeType::MultiplyMix;
	if (inString == "class Fracture::MixNode") return nodeType::Mix;
	if (inString == "class Fracture::OutlineNode") return nodeType::Outline;
	if (inString == "class Fracture::SSAONode") return nodeType::SSAO;
	if (inString == "class Fracture::ThresholdNode") return nodeType::Threshold;
	if (inString == "class Fracture::ToneMappingNode") return nodeType::ToneMapping;
	if (inString == "class Fracture::SinkNode") return nodeType::Sink;
	if (inString == "class Fracture::SourceNode") return nodeType::Source;
}

Fracture::FramaGraphSerialiser::FramaGraphSerialiser(const std::shared_ptr<FrameGraph>& graph):m_graph(graph)
{
}

void Fracture::FramaGraphSerialiser::SerialiseGraph(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["FrameGraph"] = "Temporary Graph";

	json serialised_nodes = json::array_t();
	for (const auto& node : m_graph->GetNodes())
	{			
		std::string typeName = typeid(*node).name();
		FRACTURE_INFO("Serialising Node of Type: {}", typeName);
		switch (hashit(typeName))
		{
			case nodeType::ClearFrame:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<ClearFrameSerialiser>()));
			}
			break;
			case nodeType::AdditiveMix:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<AdditiveMixNodeSerialiser>()));
			}
			break;
			case nodeType::Depth:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared < DepthNodeSerialiser>()));
				
			}
			break;
			case nodeType::GaussianBlur:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<GaussianBlurNodeSerialiser>()));
			}
			break;
			case nodeType::Intermediate:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<IntermediateNodeSerialiser>()));
			}
			break;
			case nodeType::Lambertian:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<LambertianNodeSerialiser>()));
			}
			break;
			case nodeType::MultiplyMix:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<MultiplyMixNodeSerialiser>()));
			}
			break;
			case nodeType::Outline:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<OutlineNodeSerialiser>()));
			}
			break;
			case nodeType::SSAO:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<SSAONodeSerialiser>()));
			}
			break;
			case nodeType::Threshold:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<ThresholdNodeSerialiser>()));
			}
			break;
			case nodeType::ToneMapping:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<ToneMappingNodeSerialiser>()));
			}
			break;
			case nodeType::Mix:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<MixNodeSerialiser>()));
			}
			break;
			case nodeType::Source:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<SourceNodeSerialiser>()));
			}
			break;
			case nodeType::Sink:
			{
				serialised_nodes.push_back(node->Accept(std::make_shared<SinkNodeSerialiser>()));
			}
		}
	}

	json serialised_links = json::array_t();
	for (const auto& link : m_graph->GetLinks())
	{		
		serialised_links.push_back(link->Accept(std::make_shared<LinkSerialiser>()));
	}

	j["FrameNodes"] = serialised_nodes;
	j["Links"] = serialised_links;

	m_OutputStream << j;
	m_OutputStream.close();
}

bool Fracture::FramaGraphSerialiser::DeSerialiseGraph(const std::string& filepath)
{
	std::ifstream stream(filepath);
	json input;

	if (!stream.good())
	{
		FRACTURE_ERROR("Can't read file");
		return false;
	}

	stream >> input;
	if (input.is_null())
	{
		FRACTURE_ERROR("File is either non-json file or corrupt;");
		return false;
	}

	FRACTURE_TRACE("Fracture DeSerializing Scene");
	if (exists(input, "FrameNodes"))
	{
		for (auto entity : input["FrameNodes"])
		{			
			DeserializeNode(entity);
		}
		
	}

	if (exists(input, "Links"))
	{
		for (auto entity : input["Links"])
		{			
			DeserializeLink(entity);
		}
	}
	return true;
}

void Fracture::FramaGraphSerialiser::DeserializeNode(nlohmann::json node)
{
}

void Fracture::FramaGraphSerialiser::DeserializeLink(nlohmann::json link)
{
}

