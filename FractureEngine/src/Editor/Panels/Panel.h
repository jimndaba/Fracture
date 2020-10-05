#pragma once
#ifndef PANEL_H
#define PANEl_H

#include <memory>
#include <vector>
#include <string>
#include "UIStyle.h"


namespace Fracture
{
	class Element;
	
	class Panel
	{
	public:
		Panel(std::string name);
		~Panel();

		virtual void begin();
		virtual void render() = 0;
		virtual void end();

		void AddElement(std::shared_ptr<Element> element);
		
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
		void setAlpha 				(float value);
		void setWindowPadding		(ImVec2 value);
		void setWindowRounding		(float value);
		void setWindowBorderSize	(float value);
		void setWindowMinSize   	(ImVec2 value);
		void setWindowTitleAlign	(ImVec2 value);
		void setChildRounding		(float value);
		void setChildBorderSize		(float value);
		void setPopupRounding		(float value);
		void setPopupBorderSize		(float value);
		void setFramePadding		(ImVec2 value);
		void setFrameRounding		(float value);
		void setFrameBorderSize	    (float value);
		void setItemSpacing		    (ImVec2 value);
		void setItemInnerSpacing	(ImVec2 value);
		void setIndentSpacing		(float value);
		void setScrollbarSize		(float value);
		void setScrollbarRounding	(float value);
		void setGrabMinSize		    (float value);
		void setGrabRounding		(float value);
		void setButtonTextAlign	    (ImVec2 value);
		void setSlectableTextAlign (ImVec2 value);

	private:
		std::vector<UIStyle> m_styles;
		std::vector<std::shared_ptr<Element>> m_Elements;

		std::string m_Name;
		ImGuiWindowFlags window_flags;
		bool p_open;
	};
}

#endif