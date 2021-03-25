#include "FrameGraphSerialiser.h"
#include "Core/FractureCore.h"
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

std::string EnumToString(nodeType nType)
{
	if (nType == nodeType::ClearFrame) return "0";
	if (nType == nodeType::AdditiveMix) return "1";
	if (nType == nodeType::Depth) return "4";
	if (nType == nodeType::GaussianBlur) return "3";
	if (nType == nodeType::Intermediate) return "5";
	if (nType == nodeType::Lambertian) return "6";
	if (nType == nodeType::Link) return "12";
	if (nType == nodeType::Mix) return "2";
	if (nType == nodeType::MultiplyMix) return "7";
	if (nType == nodeType::Outline) return "8";
	if (nType == nodeType::Sink) return "13";
	if (nType == nodeType::Source) return "14";
	if (nType == nodeType::SSAO) return "9";
	if (nType == nodeType::Threshold) return "10";
	if (nType == nodeType::ToneMapping) return "11";
}

Fracture::FrameGraphSerialiser::FrameGraphSerialiser(std::shared_ptr<FrameGraph>& graph, Renderer& renderer)
	:m_graph(graph),
	m_renderer(renderer)
{
}

void Fracture::FrameGraphSerialiser::SerialiseGraph(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["FactureVersion"] = Fracture_Verion;
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

bool Fracture::FrameGraphSerialiser::DeSerialiseGraph(const std::string& filepath)
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
		for (auto node : input["FrameNodes"])
		{			
			DeserializeNode(node);
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

void Fracture::FrameGraphSerialiser::DeserializeNode(nlohmann::json n)
{
	if (n["Type"] == "ClearFrame")
	{
		auto node = std::make_shared<ClearFrame>(n["Name"]);
		std::array<float, 3> vec3 = n["Color"];
		node->Color = glm::vec3(vec3[0], vec3[1], vec3[2]);
		m_graph->addnode(node);
	}
	if (n["Type"] == "AdditiveMixNode")
	{
		auto node = std::make_shared<AdditiveMixNode>(n["Name"],m_renderer.Width(),m_renderer.Height());
		m_graph->addnode(node);
	}
	if (n["Type"] == "GaussianBlurNode")
	{
		auto node = std::make_shared<GaussianBlurNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
		node->amount = n["Amount"];
		m_graph->addnode(node);
	}
	if (n["Type"] == "DepthNode")
	{
		auto node = std::make_shared<DepthNode>(n["Name"], m_renderer.Width(), m_renderer.Height(),m_renderer.m_Bucket);
		node->NearPlane = n["Near"];
		node->FarPlane = n["Far"];
		m_graph->addnode(node);
	}
	if (n["Type"] == "IntermediateNode")
	{
		auto node = std::make_shared<IntermediateNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
		m_graph->addnode(node);
	}
	if (n["Type"] == "LambertianNode")
	{
		auto node = std::make_shared<LambertianNode>(n["Name"], m_renderer.Width(), m_renderer.Height(),m_renderer.m_Bucket);
		m_graph->addnode(node);
	}
	if (n["Type"] == "SinkNode")
	{
		auto node = std::make_shared<SinkNode>(n["Name"], m_renderer.Width(),m_renderer.Height());
		m_graph->addnode(node);
	}
	if (n["Type"] == "SourceNode")
	{
		auto node = std::make_shared<SourceNode>(n["Name"]);
		m_graph->addnode(node);
	}
	if (n["Type"] == "MultiplyMix")
	{
		auto node = std::make_shared<MultiplyMixNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
		m_graph->addnode(node);
	}
	if (n["Type"] == "Outline")
	{
		auto node = std::make_shared<OutlineNode>(n["Name"], m_renderer.Width(), m_renderer.Height(),m_renderer.m_Bucket);
		m_graph->addnode(node);
	}
	if (n["Type"] == "SSAO")
	{
		auto node = std::make_shared<SSAONode>(n["Name"], m_renderer.Width(), m_renderer.Height());		
		node->radius = n["Radius"];
		node->area = n["Area"];
		node->falloff = n["Falloff"];
		node->total_strength = n["Strenght"];
		m_graph->addnode(node);
	}
	if (n["Type"] == "Threshold")
	{
		auto node = std::make_shared<ThresholdNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
		node->brightPassThreshold = n["Threshold"];
		m_graph->addnode(node);
	}
	if (n["Type"] == "ToneMapping")
	{
		auto node = std::make_shared<ToneMappingNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
		node->Gamma  = n["Gamma"];
		node->Exposure = n["Exposure"];
		m_graph->addnode(node);
	}
	if (n["Type"] == "MixNode")
	{
		auto node = std::make_shared<MixNode>(n["Name"], m_renderer.Width(), m_renderer.Height());
	
		m_graph->addnode(node);
	}
}

void Fracture::FrameGraphSerialiser::DeserializeLink(nlohmann::json l)
{
	std::string from = l["NodeFrom"];
	std::string to = l["NodeTo"];
	std::string res = l["NodeSource"];
	std::string source = l["NodeResource"];
	m_graph->addLink(from, source, to, res);
}

