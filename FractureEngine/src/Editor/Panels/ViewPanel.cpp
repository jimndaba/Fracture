#include "ViewPanel.h"
#include "Game/Game.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderTarget.h"

Fracture::ViewPanel::ViewPanel(std::string name):Panel(name)
{
}

Fracture::ViewPanel::~ViewPanel()
{
}

void Fracture::ViewPanel::setGame(Game* game)
{
    m_game = game;
}

void Fracture::ViewPanel::render()
{
    ImVec2 pos = ImGui::GetCursorScreenPos();

    //pass the texture of the FBO
    //window.getRenderTexture() is the texture of the FBO
    //the next parameter is the upper left corner for the uvs to be applied at
    //the third parameter is the lower right corner
    //the last two parameters are the UVs
    //they have to be flipped (normally they would be (0,0);(1,1) 
    //m_game->GetRenderer()->setViewport(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    ImGui::BeginChild("GameRender");
    ImGui::GetWindowDrawList()->AddImage(
        (void*)m_game->GetRenderer()->SceneRenderTarget->ID,
        ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y), ImVec2(0, 1), ImVec2(1, 0));
  
   
    //ImVec2 wsize = ImGui::GetWindowSize();
    //ImGui::Image((ImTextureID)m_game->GetRenderer()->SceneRenderTarget->ID, wsize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::EndChild();
}
