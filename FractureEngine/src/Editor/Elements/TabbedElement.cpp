#include "TabbedElement.h"


Fracture::TabbedElement::TabbedElement(std::string name):Element(name)
{
}

Fracture::TabbedElement::~TabbedElement()
{
}

void Fracture::TabbedElement::begin()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	ImGui::BeginTabBar("Utilities", tab_bar_flags);
}

void Fracture::TabbedElement::render()
{
    if (ImGui::BeginTabItem("Log"))
    {
        if (ImGui::Button("Clear"))
        {
            m_buffer.clear();
        }
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
     

        ImGui::BeginChild("Logger");
        if (copy)
        {
            ImGui::LogToClipboard();
        }

        ImGui::TextUnformatted(m_buffer.begin());

        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void Fracture::TabbedElement::end()
{
	ImGui::EndTabBar();
}


