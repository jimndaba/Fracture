#pragma once
#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include "Panel.h"
#include "imgui/imgui_impl_sdl.h"
#include "ImGuizmo/ImGuizmo.h"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>

#include "Fracture.h"


namespace Fracture
{
	
	enum class Gizmo_Mode
	{
		Translate,
		Rotate,
		Scale,
	};

	class ViewPanel : public Panel
	{

	public:
		ViewPanel(std::string name);
		~ViewPanel();

		void init();
		void setRenderer(Renderer* renderer);
		void render() override;
		void onUpdate(float dt);

		void SetImGuizmoOperation(ImGuizmo::OPERATION operation);
		ImGuizmo::OPERATION GetImGuizmoOperation() const;

		void SetImGuizmoMode(ImGuizmo::MODE mode);
		ImGuizmo::MODE GetImGuizmoMode() const;

		bool IsGizmoValid() const;

		glm::vec2 ViewportSize()
		{
			return m_ViewportSize;
		}


	private:
		Renderer* m_renderer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_Mousepos = { 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);
		std::shared_ptr<ICamera> m_camera;

		//Gizmo stuff
		//int gizmoCount = 1;
		//Gizmo_Mode m_gizmoMode;
		ImGuizmo::OPERATION currentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE currentImGuizmoMode = ImGuizmo::MODE::WORLD;
		
		static int gizmoMode;
		int lastUsing = 0;
	};

}

#endif