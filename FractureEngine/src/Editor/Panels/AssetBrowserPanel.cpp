#include "AssetBrowserPanel.h"
#include <imgui/imgui_internal.h>

Fracture::AssetBrowserPanel::AssetBrowserPanel():Panel("AssetBrowser")
{
}

Fracture::AssetBrowserPanel::~AssetBrowserPanel()
{
}

void Fracture::AssetBrowserPanel::render()
{
    static float scroll_y = 0.0f;

    ImGui::BeginChild("Assets");
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

   
    ImGui::Columns(2, "tree", true);
    ImGui::SetColumnWidth(0, 80.0F);
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
    ImGui::SetScrollY(scroll_y);
    ImGui::ImageButton(nullptr, ImVec2(64, 64));

    scroll_y = ImGui::GetScrollY();
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::EndChild();
}
