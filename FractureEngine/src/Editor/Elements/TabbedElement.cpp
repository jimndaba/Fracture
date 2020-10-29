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
	ImGui::BeginTabBar("Asstes", tab_bar_flags);
}

void Fracture::TabbedElement::render()
{
    if (ImGui::BeginTabItem("Log"))
    {

        ImGui::EndTabItem();
    }
}

void Fracture::TabbedElement::end()
{
	ImGui::EndTabBar();
}
