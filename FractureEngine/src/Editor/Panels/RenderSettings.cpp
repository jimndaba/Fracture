#include "RenderSettings.h"
#include "Rendering/Framegraph/PassLibrary/ToneMappingNode.h"
#include "Rendering/Framegraph/PassLibrary/SSAONode.h"
#include "Rendering/Framegraph/PassLibrary/GaussianBlurNode.h"
#include "Rendering/Framegraph/PassLibrary/ThresholdNode.h"
#include "Rendering/Framegraph/PassLibrary/ClearFrame.h"
#include "Rendering/Framegraph/PassLibrary/GaussianBlurNode.h"

Fracture::RenderSettingsPanel::RenderSettingsPanel(std::string name, FrameGraph& graph):Panel(name),m_graph(graph)
{
}

void Fracture::RenderSettingsPanel::render()
{
	for (auto& m_node : m_graph.GetNodes())
	{
		const auto& tone_node = std::dynamic_pointer_cast<ToneMappingNode>(m_node);
		if (tone_node)
		{
			DrawNodeSettings<ToneMappingNode>(tone_node->GetName(),[](const std::shared_ptr<ToneMappingNode>& node,FrameGraph& graph)
			{
				DrawfloatControl("Gamma", node->Gamma,1.0f,10.0f);
				DrawfloatControl("Exposure", node->Exposure, 1.0f, 10.0f);
			});
		}

		const auto& ssao_node = std::dynamic_pointer_cast<SSAONode>(m_node);
		if (ssao_node)
		{
			DrawNodeSettings<SSAONode>(ssao_node->GetName(), [](const std::shared_ptr<SSAONode>& node, FrameGraph& graph)
			{
				DrawfloatControl("Area", node->area ,- 1.0f, 1.0f);
				DrawfloatControl("Radius", node->radius,-1.0f,1.0f);
				DrawfloatControl("Strength", node->total_strength, 0.0f,20.0f);
				DrawfloatControl("Falloff", node->falloff, -1.0f, 1.0f);
			});
		}

		const auto& guassin_node = std::dynamic_pointer_cast<GaussianBlurNode>(m_node);
		if (guassin_node)
		{
			DrawNodeSettings<GaussianBlurNode>(guassin_node->GetName(), [](const std::shared_ptr<GaussianBlurNode>& node, FrameGraph& graph)
				{
					DrawIntControl("Amount", node->amount,0);
				});
		}

		const auto& threshold_node = std::dynamic_pointer_cast<ThresholdNode>(m_node);
		if (threshold_node)
		{
			DrawNodeSettings<ThresholdNode>(threshold_node->GetName(), [](const std::shared_ptr<ThresholdNode>& node, FrameGraph& graph)
				{
					DrawfloatControl("Threshold", node->brightPassThreshold, 1.0f, 10.0f);
				});
		}

		const auto& clearFrame_node = std::dynamic_pointer_cast<ClearFrame>(m_node);
		if (clearFrame_node)
		{
			DrawNodeSettings<ClearFrame>(clearFrame_node->GetName(), [](const std::shared_ptr<ClearFrame>& node, FrameGraph& graph)
				{
					glm::vec4 color = glm::vec4(node->Color, 1.0f);
					DrawColourControl("Amount",color );
					node->Color = glm::vec3(color.x, color.y, color.z);
				});
		}
		
	}
}
