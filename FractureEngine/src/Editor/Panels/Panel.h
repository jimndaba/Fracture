#pragma once
#ifndef PANEL_H
#define PANEl_H

#include <memory>
#include <vector>
#include <string>
#include "UIStyle.h"

#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include <imgui/imgui_internal.h>


namespace Fracture
{
	class Element;
	class Material;
	struct UniformValue;
	
	class Panel
	{
	public:
		Panel(const std::string& name);
		~Panel();

		virtual void begin(bool* open);
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

		static void DrawTextInputControl(const std::string& label, std::string& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawMaterialUniform(const std::string& label, UniformValue& value, float resetValue = 0.0f, float columnWidth = 150.0f);
		static void DrawColourControl(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawfloatControl(const std::string& label, float& values, float resetValue = 0.0f, const float& max = 1.0f, float columnWidth = 100.0f);
		static void DrawIntControl(const std::string& label, int& value, int resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawBoolControl(const std::string& label, bool& value, float columnWidth = 100.0f);
		static void DrawTexture2DControl(const std::string& label, uint32_t& value, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawSample2DControl(const std::string& label, const  uint32_t& value, std::shared_ptr<Fracture::Material> mMaterial, float resetValue = 0.0f, float columnWidth = 100.0f);

	private:
		std::vector<UIStyle> m_styles;
		std::vector<std::shared_ptr<Element>> m_Elements;

		std::string m_Name;
		ImGuiWindowFlags window_flags;
	
	};
}

#endif