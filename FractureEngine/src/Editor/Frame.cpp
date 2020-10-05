#include "Frame.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Panels/Panel.h"

Fracture::Frame::Frame()
{
}

Fracture::Frame::~Frame()
{
}

void Fracture::Frame::begin(SDL_Window* window)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
}

void Fracture::Frame::render()
{
    for (int i = 0; i < m_panels.size(); i++)
    {
        m_panels[i]->begin();
        m_panels[i]->render();
        m_panels[i]->end();
    }
}

void Fracture::Frame::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void Fracture::Frame::AddPanel(std::shared_ptr<Fracture::Panel> panel)
{
    if(panel)
        m_panels.push_back(panel);
}

void Fracture::Frame::RemovePanel(std::shared_ptr<Fracture::Panel>  panel)
{
}

void Fracture::Frame::clearPanel()
{
    m_panels.clear();
}

std::vector<std::shared_ptr<Fracture::Panel>> Fracture::Frame::GetElements()
{
    return std::vector<std::shared_ptr<Panel>>();
}
