#pragma once
#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include "Panel.h"
#include "imgui/imgui_impl_glfw.h"
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

	class SceneView;
	class ViewportMode;
	class RenderViewMode;
	class SSAOViewMode;
	class DepthViewMode;
	class NormalsViewMode;
	class SpecularViewMode;

	class ViewPanel : public Panel
	{

	public:
		ViewPanel(std::string name, SceneView& scenegraph, Renderer& m_renderer);
		~ViewPanel();

		void init();
		void setRenderer(Renderer& renderer);
		void render() override;
		void onUpdate(float dt);

		void SetViewMode(const std::string& mode);

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
		Renderer& m_renderer;
		static bool m_showViewport;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_MousePicking = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_Mousepos = { 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);
		std::shared_ptr<ICamera> m_camera;

		std::shared_ptr<ViewportMode> m_Viewportmode;

		std::map<std::string, std::shared_ptr<ViewportMode>> m_viewModes;

		std::shared_ptr<RenderViewMode> m_RenderView;
		std::shared_ptr<SSAOViewMode> m_SSAOView;
		std::shared_ptr<DepthViewMode> m_DepthView;
		std::shared_ptr<NormalsViewMode> m_NormalView;
		std::shared_ptr <SpecularViewMode> m_SpecularView;

		//Gizmo stuff
		//int gizmoCount = 1;
		//Gizmo_Mode m_gizmoMode;
		ImGuizmo::OPERATION currentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE currentImGuizmoMode = ImGuizmo::MODE::WORLD;

		SceneView& m_scenegraph;
		static int gizmoMode;
		static std::string m_currentViewMode;
		int lastUsing = 0;
	};

}

#endif