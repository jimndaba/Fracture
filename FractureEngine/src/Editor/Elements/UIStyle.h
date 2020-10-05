#pragma once
#ifndef UISTYLE_H
#define UISTYLE_H
#include "imgui/imgui.h"
#include <string>

namespace Fracture
{

	enum class StyleType
	{
		Alpha = ImGuiStyleVar_Alpha,
		WindowPadding = ImGuiStyleVar_WindowPadding,
		WindowRounding = ImGuiStyleVar_WindowRounding,
		WindowBorderSize = ImGuiStyleVar_WindowBorderSize,
		WindowMinSize = ImGuiStyleVar_WindowMinSize,
		WindowTitleAlign = ImGuiStyleVar_WindowTitleAlign,
		ChildRounding = ImGuiStyleVar_ChildRounding,
		ChildBorderSize = ImGuiStyleVar_ChildBorderSize,
		PopupRounding = ImGuiStyleVar_PopupRounding,
		PopupBorderSize = ImGuiStyleVar_PopupBorderSize,
		FramePadding = ImGuiStyleVar_FramePadding,
		FrameRounding = ImGuiStyleVar_FrameRounding,
		FrameBorderSize = ImGuiStyleVar_FrameBorderSize,
		ItemSpacing = ImGuiStyleVar_ItemSpacing,
		ItemInnerSpacing = ImGuiStyleVar_ItemInnerSpacing,
		IndentSpacing = ImGuiStyleVar_IndentSpacing,
		ScrollbarSize = ImGuiStyleVar_ScrollbarSize,
		ScrollbarRounding = ImGuiStyleVar_ScrollbarRounding,
		GrabMinSize = ImGuiStyleVar_GrabRounding,
		TabRounding = ImGuiStyleVar_TabRounding,
		GrabRounding = ImGuiStyleVar_TabRounding,
		ButtonTextAlign = ImGuiStyleVar_ButtonTextAlign,
		SelectableTextAlign = ImGuiStyleVar_SelectableTextAlign,
	};

	enum class ValueType
	{
		Float,
		Vec2,
	};

	struct UIStyle
	{
		StyleType type;		
		ValueType varType;
		union
		{
			float Float;
			ImVec2 Vec2;
		};
		UIStyle() {};

	};

}

#endif