#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Panel.h"
#include "Fracture.h"
#include <imgui/imgui_internal.h>

namespace Fracture
{
	class InspectorPanel: public Panel
	{
	public:
		InspectorPanel(std::string name);
		~InspectorPanel();

		void render() override;
		

	private:
		void DrawComponents(Entity entity);

		template<class T, typename UIFunction>
		void DrawComponent(const std::string& name,Entity entity, UIFunction uiFunction);

		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawColourControl(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawfloatControl(const std::string& label, float& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawIntControl(const std::string& label, int& value, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawTexture2DControl(const std::string& label, unsigned int& value, float resetValue = 0.0f, float columnWidth = 100.0f);
	};

	template<class T, typename UIFunction>
	inline void InspectorPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (Fracture::ComponentManager::HasComponent<T>(entity.Id))
		{
			std::shared_ptr<T> component = ComponentManager::GetComponent<T>(entity.Id);
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				ComponentManager::RemoveComponent<T>(entity.Id);
			}
				
		}

	}

}

#endif