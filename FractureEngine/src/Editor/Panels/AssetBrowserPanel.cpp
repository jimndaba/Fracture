#include "AssetBrowserPanel.h"
#include <imgui/imgui_internal.h>
#include "../utils/FileDialogue.h"
#include "Math/Math.h"
#include "../Factories/MaterialFactory.h"

bool Fracture::AssetBrowserPanel::m_showModels;
bool Fracture::AssetBrowserPanel::m_showTextures;
bool Fracture::AssetBrowserPanel::m_showScenes;
bool Fracture::AssetBrowserPanel::m_showMaterials;
bool Fracture::AssetBrowserPanel::m_showShaders;
float Fracture::AssetBrowserPanel::scroll_y = 0.0f;

Fracture::AssetBrowserPanel::AssetBrowserPanel():Panel("AssetBrowser")
{
}

Fracture::AssetBrowserPanel::~AssetBrowserPanel()
{
}

void Fracture::AssetBrowserPanel::render()
{
  

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
            AssetManager::AddTexture2D(name, filepath,TextureType::Diffuse);
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

    ImGuiSelectableFlags flags = ImGuiSelectableFlags_SelectOnClick | ImGuiSelectableFlags_SpanAvailWidth;
   
    ImGui::Columns(2, "tree", true);
    ImGui::SetColumnWidth(0, 80.0F);
    ImGui::Selectable("Models", &m_showModels,flags);    
    ImGui::Selectable("Textures", &m_showTextures, flags);
    ImGui::Selectable("Materials",&m_showMaterials,flags); 
    ImGui::Selectable("Shaders",&m_showShaders,flags);
    ImGui::Selectable("Scenes",&m_showScenes,flags);

    int iconSize = 64;
    int offset = 30;

    if (m_showModels)
    {
        ImVec2 region = ImGui::GetContentRegionAvail();
        int iconSize = 64;
        int offset = 30;
        int columns = region.x / (iconSize + offset);
    }

    if (m_showTextures)
    {
        ImGui::NextColumn();
        ImGui::BeginChild("AssetViewer");
        ImGui::SetScrollY(scroll_y);     
        ImVec2 region = ImGui::GetContentRegionAvail();
        
        int columns = region.x / (iconSize + offset);

        const auto& m_textures =  AssetManager::GetTextures();

        if (ImGui::BeginTable("split1", columns, m_tableflags))
        {
            for (auto& texture : m_textures)
            {
                ImGui::ImageButton((void*)texture.second->GetTextureID(), ImVec2(iconSize, iconSize), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                ImGui::Text(texture.first.c_str());
                ImGui::TableNextColumn();
            }
            ImGui::EndTable();
        } 
        ImGui::EndChild();
    }

    if (m_showShaders)
    {
        ImGui::NextColumn();
        ImGui::BeginChild("AssetViewer");
        ImGui::SetScrollY(scroll_y);
        ImVec2 region = ImGui::GetContentRegionAvail();
      
        int columns = region.x / (iconSize + offset);

        const auto& m_shaders = AssetManager::GetShaders();
    
        if (ImGui::BeginTable("split1",columns, m_tableflags))
        {
            for (auto& shader : m_shaders)
            {               
                ImGui::ImageButton((void*)AssetManager::getTexture2D("ShaderIcon")->GetTextureID(), ImVec2(iconSize, iconSize), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                ImGui::Text(shader->Name.c_str());
                ImGui::TableNextColumn();
            }
            ImGui::EndTable();
        }
        scroll_y = ImGui::GetScrollY();
        ImGui::EndChild();
    }

    if (m_showScenes)
    {
        ImGui::NextColumn();
        ImGui::BeginChild("AssetViewer");
        ImGui::SetScrollY(scroll_y);
        ImVec2 region = ImGui::GetContentRegionAvail();
        
        int columns = region.x / (iconSize + offset);

        const auto& m_scenes =SceneManager::GetScenes();

        if (ImGui::BeginTable("split1", columns, m_tableflags))
        {
            for (auto& scene : m_scenes)
            {
                ImGui::ImageButton((void*)AssetManager::getTexture2D("SceneIcon")->GetTextureID(), ImVec2(iconSize, iconSize), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                ImGui::Text(scene.first.c_str());
                ImGui::TableNextColumn();
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }

    if (m_showMaterials)
    {
        ImGui::NextColumn();
        ImGui::BeginChild("AssetViewer");
        ImGui::SetScrollY(scroll_y);
        ImVec2 region = ImGui::GetContentRegionAvail();
      
        int columns = region.x / (iconSize + offset);
   
        const auto& m_materials = AssetManager::GetMaterials();

        if (ImGui::BeginTable("split1", columns,m_tableflags))
        {
            for (auto& material : m_materials)
            {
                ImGui::ImageButton((void*)AssetManager::getTexture2D("MaterialIcon")->GetTextureID(), ImVec2(iconSize, iconSize), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
                ImGui::Text(material.first.c_str());
                ImGui::TableNextColumn();
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }    

   
    ImGui::Columns(1);
    ImGui::EndChild();
}
