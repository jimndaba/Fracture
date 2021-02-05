#include "Panel.h"
#include "../Elements/Element.h"

bool* Fracture::Panel::p_open;

Fracture::Panel::Panel(const std::string& name):m_Name(name)
{
	p_open = new bool(true);
}

Fracture::Panel::~Panel()
{
}

void Fracture::Panel::begin()
{
	for (int i = 0; i < m_styles.size(); i++)
	{
		if (m_styles[i].varType == ValueType::Float)
		{
			ImGui::PushStyleVar((ImGuiStyleVar)m_styles[i].type, m_styles[i].Float);
		}
		if(m_styles[i].varType == ValueType::Vec2)
		{
			ImGui::PushStyleVar((ImGuiStyleVar)m_styles[i].type, m_styles[i].Vec2);
		}
	}
	ImGui::Begin(m_Name.c_str(),p_open);
	for (int i = 0; i < m_styles.size(); i++)
	{
		ImGui::PopStyleVar();
	}
}

void Fracture::Panel::end()
{
	for (int i = 0; i < m_Elements.size(); i++)
	{
		m_Elements[i]->begin();
		m_Elements[i]->render();
		m_Elements[i]->end();
	}

	ImGui::End();
}

void Fracture::Panel::AddElement(std::shared_ptr<Element> element)
{
	m_Elements.push_back(element);
}

void Fracture::Panel::setAlpha(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::Alpha;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowPadding(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::WindowPadding;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::WindowRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowBorderSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::WindowBorderSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowMinSize(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::WindowMinSize;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowTitleAlign(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::WindowTitleAlign;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setChildRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::ChildRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setChildBorderSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::ChildBorderSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setPopupRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::PopupRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setPopupBorderSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::PopupBorderSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFramePadding(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.varType = ValueType::Float;
	style.type = StyleType::FramePadding;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFrameRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::FrameRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFrameBorderSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::FrameBorderSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setItemSpacing(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::ItemSpacing;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setItemInnerSpacing(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::ItemInnerSpacing;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setIndentSpacing(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::IndentSpacing;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setScrollbarSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::ScrollbarSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setScrollbarRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::ScrollbarRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setGrabMinSize(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::GrabMinSize;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setGrabRounding(float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = StyleType::GrabRounding;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setButtonTextAlign(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::ButtonTextAlign;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setSlectableTextAlign(ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = StyleType::SelectableTextAlign;
	style.Vec2 = value;
	m_styles.push_back(style);
}
