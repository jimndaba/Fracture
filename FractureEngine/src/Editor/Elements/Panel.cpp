#include "Panel.h"

Fracture::Panel::Panel(std::string name)
{
	p_open = true;
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
	ImGui::Begin("Scene",(bool*)p_open);
	for (int i = 0; i < m_styles.size(); i++)
	{
		ImGui::PopStyleVar();
	}
}

void Fracture::Panel::end()
{
	ImGui::End();
}

void Fracture::Panel::setAlpha(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowPadding(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowMinSize(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setWindowTitleAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setChildRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setChildBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setPopupRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setPopupBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFramePadding(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.varType = ValueType::Float;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFrameRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setFrameBorderSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setItemSpacing(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setItemInnerSpacing(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setIndentSpacing(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setScrollbarSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setScrollbarRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setGrabMinSize(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setGrabRounding(StyleType type, float value)
{
	UIStyle style;
	style.varType = ValueType::Float;
	style.type = type;
	style.Float = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setButtonTextAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}

void Fracture::Panel::setSlectableTextAlign(StyleType type, ImVec2 value)
{
	UIStyle style;
	style.varType = ValueType::Vec2;
	style.type = type;
	style.Vec2 = value;
	m_styles.push_back(style);
}
