#include "TabbedPanel.h"
#include "../Elements/TabbedElement.h"

Fracture::TabbedPanel::TabbedPanel(std::string name):Panel(name)
{
    m_tabElement = std::shared_ptr<TabbedElement>(new TabbedElement("low panel"));

    AddElement(m_tabElement);
}

Fracture::TabbedPanel::~TabbedPanel()
{
}

void Fracture::TabbedPanel::render()
{
   
}
