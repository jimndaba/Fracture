#include "TabbedElement.h"
#include <imgui/imgui_internal.h>

Fracture::TabbedElement::TabbedElement(std::string name):Element(name)
{
}

Fracture::TabbedElement::~TabbedElement()
{
}

void Fracture::TabbedElement::begin()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	ImGui::BeginTabBar("Asstes", tab_bar_flags);
}

void Fracture::TabbedElement::render()
{
    if (ImGui::BeginTabItem("Assets"))
    {
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 50.0f, lineHeight };
        if (ImGui::Button("AddModel", buttonSize))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("AddTexture", buttonSize))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("AddShader", buttonSize))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("AddMaterial", buttonSize))
        {

        }

        ImGui::BeginChild("Assets");
        ImGui::Columns(2, "tree", true);    
        if (ImGui::Selectable("Models"))
        {

        }
        if (ImGui::Selectable("Textures"))
        {

        }
        if (ImGui::Selectable("Materials"))
        {

        }
        if (ImGui::Selectable("Shaders"))
        {

        }

      
        ImGui::NextColumn();
        ImGui::BeginChild("AssetViewer");
        ImGui::ImageButton(nullptr, ImVec2(64, 64));
        ImGui::EndChild();        
        ImGui::NextColumn();
       
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Log"))
    {

        ImGui::EndTabItem();
    }
}

void Fracture::TabbedElement::end()
{
	ImGui::EndTabBar();
}
