#pragma once
#ifndef EDITOR_H
#define EDITOR_H
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Fracture.h"
#include <memory>

namespace Fracture
{

	class Frame;
	class SceneView;
	class InspectorPanel;
	class ViewPanel;
	class TabbedPanel;


	class Editor
	{
	public:
		Editor();
		~Editor();

		void onInit();
		void run();
		void onUpdate();
		void onShutdown();
		void Render();

		void SetScene(std::shared_ptr<Scene> scene);

		bool done;

		static std::shared_ptr<Scene> ActiveScene();

	private:

		std::unique_ptr<Logger> m_logger;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<GameWindow> m_window;
		std::unique_ptr<Eventbus> m_Eventbus;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<AssetManager> m_AssetMaanger;

		static std::shared_ptr<Scene> m_ActiveScene;


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags panel_flags = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		std::shared_ptr<Frame> m_frame;
		std::shared_ptr<SceneView> m_sceneview;
		std::shared_ptr<InspectorPanel> m_inspectorpanel;
		std::shared_ptr<ViewPanel> m_viewpanel;
		std::shared_ptr<TabbedPanel> m_TabbedPanel;

		static bool p_open;
		static bool opt_fullscreen;
		static bool opt_padding;		
		int display_width, display_height;
	};
}

#endif