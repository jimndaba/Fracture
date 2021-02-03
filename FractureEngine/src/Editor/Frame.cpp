#include "Frame.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Panels/Panel.h"
#include "Profiling/Profiler.h"

Fracture::Frame::Frame()
{
}

Fracture::Frame::~Frame()
{
}

void Fracture::Frame::begin(GLFWwindow* window)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void Fracture::Frame::render()
{
    ProfilerTimer timer("Render Frame");
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
       
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
     
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
    return m_panels;
}
