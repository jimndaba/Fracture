#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Panel.h"
#include "Fracture.h"
#include <imgui/imgui_internal.h>

namespace Fracture
{
	struct UniformValue;
	class SceneView;

	enum class SampleUniformType
	{
		Diffuse = 3,
		Normal,
		Metallic,
		Roughness,
		AO,
		Specular,
		Emmision,
		Height,
		Bump,
		Reflection,
	};

	class InspectorPanel: public Panel
	{
	public:
		InspectorPanel(std::string name, SceneView& scenegraph);
		~InspectorPanel();

		void render() override;
		

		static void DrawTextInputControl(const std::string& label, std::string& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawMaterialUniform(const std::string& label, UniformValue& value, float resetValue = 0.0f, float columnWidth = 150.0f);
		static void DrawColourControl(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawfloatControl(const std::string& label, float& values, float resetValue = 0.0f, const float& max =1.0f, float columnWidth = 100.0f);
		static void DrawIntControl(const std::string& label, int& value, int resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawBoolControl(const std::string& label, bool& value, float columnWidth = 100.0f);
		static void DrawTexture2DControl(const std::string& label, unsigned int& value, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawSample2DControl(const std::string& label, unsigned int& value, std::shared_ptr<Fracture::Material> mMaterial, float resetValue = 0.0f, float columnWidth = 100.0f);
	private:
		
		SceneView& m_scenegraph;
		void DrawComponents(Entity entity);

		template<class T, typename UIFunction>
		void DrawComponent(const std::string& name,Entity entity, UIFunction uiFunction);

		
	};

	template<class T, typename UIFunction>
	inline void InspectorPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	
		if (Fracture::ComponentManager::HasComponent<T>(entity.Id))
		{
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			std::shared_ptr<T> component = ComponentManager::GetComponent<T>(entity.Id);
		
			ImGuiWindowFlags window_flags =ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar| ImGuiWindowFlags_AlwaysAutoResize;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.0f);			
			

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
				ImGui::Separator();
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (removeComponent)
			{
				ComponentManager::RemoveComponent<T>(entity.Id);
			}
		
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);
		}		
	}

}

#endif