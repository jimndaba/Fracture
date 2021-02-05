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
		Panel(const std::string& name);
		~Panel();

		virtual void begin();
		virtual void render() = 0;
		virtual void end();

		void AddElement(std::shared_ptr<Element> element);
				
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
		static bool* p_open;
	};
}

#endif