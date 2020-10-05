#include "Inspector.h"
#include "../Elements/TagComponentElement.h"

Fracture::InspectorPanel::InspectorPanel(std::string name):Panel(name)
{
	m_tagcomponent = std::shared_ptr< TagComponentElement>(new TagComponentElement("Tag"));

	AddElement(m_tagcomponent);
}

Fracture::InspectorPanel::~InspectorPanel()
{
}

void Fracture::InspectorPanel::render()
{
}
