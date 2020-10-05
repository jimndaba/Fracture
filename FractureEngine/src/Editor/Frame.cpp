#include "Frame.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Elements/Element.h"

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
    for (int i = 0; i < m_elements.size(); i++)
    {
        m_elements[i]->render();
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

void Fracture::Frame::AddElement(std::shared_ptr<Element>  element)
{
    if(element)
        m_elements.push_back(element);
}

void Fracture::Frame::RemoveElement(std::shared_ptr<Element>  element)
{
}

std::vector<std::shared_ptr<Fracture::Element>> Fracture::Frame::GetElements()
{
    return std::vector<std::shared_ptr<Element>>();
}
