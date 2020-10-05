#include "Element.h"

Fracture::Element::Element(std::string name)
{
	p_open = true;
}

Fracture::Element::~Element()
{
}

void Fracture::Element::begin()
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
	ImGui::Begin("Scene",(bool*)p_open);
	for (int i = 0; i < m_styles.size(); i++)
	{
		ImGui::PopStyleVar();
	}
}

void Fracture::Element::end()
{
	ImGui::End();
}

void Fracture::Element::setAlpha(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setWindowPadding(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setWindowRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setWindowBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setWindowMinSize(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setWindowTitleAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setChildRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setChildBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setPopupRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setPopupBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setFramePadding(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.varType = ValueType::Float;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setFrameRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setFrameBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setItemSpacing(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setItemInnerSpacing(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setIndentSpacing(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setScrollbarSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setScrollbarRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setGrabMinSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setGrabRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Element::setButtonTextAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Element::setSlectableTextAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}
