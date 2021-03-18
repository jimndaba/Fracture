#pragma once
#ifndef RENDERSETTINGS_H
#define RENDERSETTIGNS_H

#include "Panel.h"
#include "imgui/imgui_impl_glfw.h"
#include "ImGuizmo/ImGuizmo.h"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>


#include "Fracture.h"

namespace Fracture
{
	class FrameGraph;
	class FrameNode;

	class RenderSettingsPanel : public Panel
	{

	public:
		RenderSettingsPanel(std::string name, FrameGraph& graph);
		~RenderSettingsPanel() = default;

		void render() override;

		template<class T, typename UIFunction>
		void DrawNodeSettings(const std::string& name, UIFunction uiFunction);

	private:
		FrameGraph& m_graph;
	};
	template<class T, typename UIFunction>
	inline void RenderSettingsPanel::DrawNodeSettings(const std::string& name,UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		const auto& node = std::dynamic_pointer_cast<T>(m_graph.getNode(name));
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.0f);

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 1.0f;
		
		//ImGui::PushID(name.c_str());
		bool open = ImGui::TreeNodeEx(name.c_str(), treeNodeFlags, node->GetName().c_str());//	(void*)typeid(T).hash_code()
		//ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

		if (open)
		{
			uiFunction(node, m_graph);
			ImGui::TreePop();
			ImGui::Separator();
		}
		//ImGui::PopID();

		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}
}

#endif