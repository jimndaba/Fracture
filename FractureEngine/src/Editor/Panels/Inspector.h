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
	
		if (Fracture::ComponentManager::HasComponent<T>(entity.GetId()))
		{
			
			
			auto& components = ComponentManager::GetComponents<T>(entity.GetId()); 
					
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.0f);

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 1.0f;
			for(int i=0;i<components.size();i++)
			{ 
				ImGui::PushID(i);
						
				bool open = ImGui::TreeNodeEx(typeid(T).name() + i, treeNodeFlags, name.c_str());//(void*)typeid(T).hash_code()		
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				if (open)
				{
					uiFunction(components[i]);
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
					auto c = std::dynamic_pointer_cast<T>(components[i]);
					ComponentManager::RemoveComponent<T>(c);
				}
							
				ImGui::PopID();
			}

			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(2);
		}		
	}

}

#endif