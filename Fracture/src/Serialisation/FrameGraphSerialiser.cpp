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

Fracture::FramaGraphSerialiser::FramaGraphSerialiser(const std::shared_ptr<FrameGraph>& graph):m_graph(graph)
{
}

void Fracture::FramaGraphSerialiser::SerialiseGraph(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["FrameGraph"] = "Temporary Graph";

	for (const auto& node : m_graph->GetNodes())
	{
		auto m_node = std::static_pointer_cast<ClearFrame>(node);
		if (m_node)
		{
			node->Accept(new ClearFrameSerialiser(j));
		}
		
	}

}

void Fracture::FramaGraphSerialiser::DeSerialiseGraph()
{
}
