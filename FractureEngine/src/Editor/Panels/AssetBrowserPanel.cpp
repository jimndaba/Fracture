#include "AssetBrowserPanel.h"
#include <imgui/imgui_internal.h>
#include "../utils/FileDialogue.h"
#include "Math/Math.h"
#include "../Factories/MaterialFactory.h"

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
        std::string name;
        std::string filepath = FileDialogue::OpenFile("fbx(*.fbx)\0*.fbx\0obj(*.obj)\0*.obj\0blender(*.blend)\0*.blend\0", name);
        if (!filepath.empty())
        {
            AssetManager::AddModel(name, filepath);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("AddTexture", buttonSize))
    {
        std::string name;
        std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0tga(*.tga)\0*.tga\0bmp(*.bmp)\0*.bmp\0hdr(*.hdr)\0*.hdr\0", name);
        if (!filepath.empty())
        {
            AssetManager::AddTexture(name, filepath,TextureType::Diffuse);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("AddShader", buttonSize))
    {
        ImGui::OpenPopup("Add new shader?");
    }

    ImGui::SameLine();

    if (ImGui::Button("AddMaterial", buttonSize))  
    {
        ImGui::OpenPopup("Creat new Material?");  
    }

    if (ImGui::BeginPopupModal("Add new shader?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string shader;
        static std::string vertex;
        static std::string fragment;

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 100.0f, lineHeight };

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), shader.c_str());

        ImGui::Text("Shader name: \n");
        ImGui::SameLine();
        if (ImGui::InputText("##material", buffer, sizeof(buffer)))
        {
            shader = std::string(buffer);
        }
        
        ImGui::Text("Vertex Shader: \n");
        ImGui::SameLine();
        if (ImGui::Button("add vertex", buttonSize))
        {
            vertex = FileDialogue::OpenFile("glsl(*.glsl)\0*.glsl\0vertex(*.vert)\0*.vert\0", vertex);
        }       
        ImGui::Separator();
        
        ImGui::Text("Fragment Shader: \n");
        ImGui::SameLine();
        if (ImGui::Button("add fragment", buttonSize))
        {
            fragment = FileDialogue::OpenFile("glsl(*.glsl)\0*.glsl\0fragment(*.frag)\0*.frag\0", fragment);
        }

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            if (!shader.empty()&& !vertex.empty() && !fragment.empty())
            {
                AssetManager::AddShader(shader,vertex,fragment);
                ImGui::CloseCurrentPopup();
            }
            else
            {
                FRACTURE_ERROR("Could Not load shader: {} " , shader);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Creat new Material?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    { 
        ImGui::Text("Material Name: .\n");
        ImGui::SameLine();
        static std::string name;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer),name.c_str());

        if (ImGui::InputText("##material", buffer, sizeof(buffer)))
        {
            name = std::string(buffer);
        }
        
        if (ImGui::Button("OK", ImVec2(120, 0))) 
        { 
            if (!name.empty())
            {
                std::shared_ptr<Material> material = MaterialFactory::PBRMaterial(name);
                AssetManager::AddMaterial(name, material);
                ImGui::CloseCurrentPopup();
            }            
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
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
    ImGui::Image((void*)AssetManager::getTexture("GameObjectIcon")->id, ImVec2(16, 16));

    scroll_y = ImGui::GetScrollY();
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::EndChild();
}
