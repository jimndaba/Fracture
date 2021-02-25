#include "Panel.h"
#include "../Elements/Element.h"
#include "../Elements/TagComponentElement.h"
#include "SceneviewPanel.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "../utils/FileDialogue.h"

Fracture::Panel::Panel(const std::string& name):m_Name(name)
{

}

Fracture::Panel::~Panel()
{
}

void Fracture::Panel::begin(bool* open)
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
	ImGui::Begin(m_Name.c_str(),open);
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

void Fracture::Panel::DrawTextInputControl(const std::string& label, std::string& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), values.c_str());
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
	{
		std::string str(buffer);
		values = str;
	}

	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::Panel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::AlignTextToFramePadding();
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::SameLine();
	ImGui::PopStyleColor(4);


	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	int push = 2;
	const bool buttonHovered = ImGui::IsItemHovered();
	if (buttonHovered)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.00f));
		push = 3;
	}


	ImGui::PopStyleColor(push);
	ImGui::PopFont();

	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PopStyleColor(5);


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.34f, 0.58f, 0.97f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.6f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.34f, 0.58f, 0.97f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;


	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::PopFont();
	ImGui::PopStyleColor(6);

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::Panel::DrawMaterialUniform(const std::string& label, UniformValue& value, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;

	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());

	//ImGui::Columns(2);
	ImGui::SetColumnWidth(1, columnWidth);

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), value.Name.c_str());
	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("##uniformText", buffer, sizeof(buffer), flags))
	{
		std::string str(buffer);
		value.Name = str;
	}
	ImGui::PopItemWidth();
	ImGui::NextColumn();


	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	switch (value.Type)
	{
	case SHADER_TYPE_BOOL:
	{
		ImGui::Checkbox(label.c_str(), &value.Bool);

		break;
	}
	case SHADER_TYPE_INT:
	{
		ImGui::PushFont(boldFont);
		if (ImGui::Button("-", buttonSize))
			value.Int = (int)resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::InputInt("##X", &value.Int);
		break;
	}
	case SHADER_TYPE_FLOAT:
	{
		ImGui::PushFont(boldFont);
		if (ImGui::Button("-", buttonSize))
			value.Float = resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##uniform", &value.Float, 0.001f, 0.0f, 10.0f, "%.2f");

		break;
	}
	case SHADER_TYPE_VEC2:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec2.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec2.x, 0.001f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec2.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec2.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		break;
	}
	case SHADER_TYPE_VEC3:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec3.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec3.x, 0.05f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec3.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec3.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.Vec3.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.Vec3.z, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		break;
	}
	case SHADER_TYPE_VEC4:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec4.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec4.x, 0.05f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec4.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec4.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.Vec4.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.Vec4.z, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.84f, 0.84f, 0.84f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.95f, 0.95f, 0.95f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("w", buttonSize))
			value.Vec4.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##w", &value.Vec4.w, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		break;
	}
	case SHADER_TYPE_COLOR3:
	{
		glm::vec4 color = glm::vec4(value.Color3, 1.0f);
		DrawColourControl(label, color, 1.0f);
		value.Color3 = glm::vec3(color.x, color.y, color.z);
		break;
	}
	case SHADER_TYPE_COLOR4:
	{
		DrawColourControl(label, value.Color4, 1.0f);
		break;
	}
	case SHADER_TYPE_MAT2:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	case SHADER_TYPE_MAT3:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	case SHADER_TYPE_MAT4:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	default:
		//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
		break;
	}


	ImGui::Columns(1);
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::Panel::DrawColourControl(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	float color4[4] = { values.x,values.y,values.z,values.w };
	ImGui::PushID("##color");
	ImGui::ColorEdit4(label.c_str(), (float*)&color4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	values = glm::vec4(color4[0], color4[1], color4[2], color4[3]);
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::Panel::DrawfloatControl(const std::string& label, float& value, float resetValue, const float& max, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushFont(boldFont);
	if (ImGui::Button("-", buttonSize))
		value = resetValue;
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::DragFloat("##uniform", &value, 0.01f, 0.0f, max, "%.2f");
	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::Panel::DrawIntControl(const std::string& label, int& value, int resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushFont(boldFont);
	if (ImGui::Button("-", buttonSize))
		value = resetValue;
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::InputInt("##X", &value);
	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::Panel::DrawBoolControl(const std::string& label, bool& value, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID("##bool");
	ImGui::PushFont(boldFont);
	ImGui::Checkbox(label.c_str(), &value);
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::Panel::DrawTexture2DControl(const std::string& label, uint32_t& value, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImTextureID texture = (void*)value;
	ImGui::PushID("##sample");
	if (ImGui::ImageButton(texture, ImVec2(64, 64)) && ImGui::IsMouseDoubleClicked(0))
	{
		std::string name;
		std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0bmp(*.bmp)\0*.bmp\0", name);
		if (!filepath.empty())
		{
			AssetManager::AddTexture2D(name, filepath, TextureType::Diffuse);
			value = AssetManager::getTexture2D(name)->GetTextureID();
		}
	}
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::Panel::DrawSample2DControl(const std::string& label, const uint32_t& value, std::shared_ptr<Fracture::Material> mMaterial, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImTextureID texture = (void*)value;
	ImGui::PushID("##sample");
	if (ImGui::ImageButton(texture, ImVec2(64, 64)))
	{
		std::string name;
		std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0bmp(*.bmp)\0*.bmp\0", name);
		if (!filepath.empty())
		{
			AssetManager::AddTexture2D(name, filepath, TextureType::Diffuse);
			mMaterial->ChangeTexture(label, AssetManager::getTexture2D(name), (int)AssetManager::getTexture2D(name)->GetType());
		}

	}
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}
