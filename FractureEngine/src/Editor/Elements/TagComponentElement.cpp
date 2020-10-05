#include "TagComponentElement.h"


Fracture::TagComponentElement::TagComponentElement(std::string name):Element(name)
{
}

Fracture::TagComponentElement::~TagComponentElement()
{
}

void Fracture::TagComponentElement::begin()
{
	ImGui::Separator();
	ImGui::BeginChild(Name().c_str(),ImVec2(0,50));
}

void Fracture::TagComponentElement::render()
{
	ImGui::Text("ID: %d", 10);
	//ImGui::SameLine();
	static char str0[128] = "Cube";
	ImGui::InputText("Asset Name: ", str0, IM_ARRAYSIZE(str0));
}

void Fracture::TagComponentElement::end()
{
	//ImGui::End();
	ImGui::EndChild();
	ImGui::Separator();
}
