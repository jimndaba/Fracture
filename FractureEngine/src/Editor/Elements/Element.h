#pragma once
#ifndef ELEMENT_H
#define ELEMENT_H

#include <memory>
#include <vector>
#include <string>
#include "UIStyle.h"


namespace Fracture
{
	
	class Element
	{
	public:
		Element(std::string name);
		~Element();

		void begin();
		virtual void render() = 0;
		void end();
		
		/*
		ImGuiStyleVar_Alpha,               // float     Alpha
		ImGuiStyleVar_WindowPadding,       // ImVec2    WindowPadding
		ImGuiStyleVar_WindowRounding,      // float     WindowRounding
		ImGuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
		ImGuiStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
		ImGuiStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
		ImGuiStyleVar_ChildRounding,       // float     ChildRounding
		ImGuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
		ImGuiStyleVar_PopupRounding,       // float     PopupRounding
		ImGuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
		ImGuiStyleVar_FramePadding,        // ImVec2    FramePadding
		ImGuiStyleVar_FrameRounding,       // float     FrameRounding
		ImGuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
		ImGuiStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
		ImGuiStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
		ImGuiStyleVar_IndentSpacing,       // float     IndentSpacing
		ImGuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
		ImGuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
		ImGuiStyleVar_GrabMinSize,         // float     GrabMinSize
		ImGuiStyleVar_GrabRounding,        // float     GrabRounding
		ImGuiStyleVar_TabRounding,         // float     TabRounding
		ImGuiStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
		ImGuiStyleVar_SelectableTextAlign, // ImVec2    SelectableTextAlign
		*/
		void setAlpha 				(StyleType type, float value);
		void setWindowPadding		(StyleType type, ImVec2 value);
		void setWindowRounding		(StyleType type, float value);
		void setWindowBorderSize	(StyleType type, float value);
		void setWindowMinSize   	(StyleType type, ImVec2 value);
		void setWindowTitleAlign	(StyleType type, ImVec2 value);
		void setChildRounding		(StyleType type, float value);
		void setChildBorderSize		(StyleType type, float value);
		void setPopupRounding		(StyleType type, float value);
		void setPopupBorderSize		(StyleType type, float value);
		void setFramePadding		(StyleType type, ImVec2 value);
		void setFrameRounding		(StyleType type, float value);
		void setFrameBorderSize	    (StyleType type, float value);
		void setItemSpacing		    (StyleType type, ImVec2 value);
		void setItemInnerSpacing	(StyleType type, ImVec2 value);
		void setIndentSpacing		(StyleType type, float value);
		void setScrollbarSize		(StyleType type, float value);
		void setScrollbarRounding	(StyleType type, float value);
		void setGrabMinSize		    (StyleType type, float value);
		void setGrabRounding		(StyleType type, float value);
		void setButtonTextAlign	    (StyleType type, ImVec2 value);
		void setSlectableTextAlign (StyleType type, ImVec2 value);

	private:
		std::vector<UIStyle> m_styles;
		std::string m_Name;

		bool p_open;
	};
}

#endif