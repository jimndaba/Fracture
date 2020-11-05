#pragma once
#ifndef EDITOR_H
#define EDITOR_H
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
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
	class AssetBrowserPanel;
	class EntityFactory;

	class Editor
	{
	public:
		Editor();
		~Editor();

		void onInit();
		void run();
		void onUpdate(float dt);
		void onRender();
		void onShutdown();
		void Render();
		void SetScene();

		bool done;

		void DrawMenuBar();

		static std::shared_ptr<Scene> ActiveScene();

	private:

		std::unique_ptr<Logger> m_logger;
		std::unique_ptr<Profiler> m_Profiler;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<GameWindow> m_window;
		std::unique_ptr<Eventbus> m_Eventbus;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<AssetManager> m_AssetManger;
		std::unique_ptr<PhysicsManager> m_PhysicsManger;
		static std::unique_ptr<SceneManager> m_SceneManager;
		static std::unique_ptr<EntityFactory> m_EntityFactory;
		static std::shared_ptr<Scene> m_ActiveScene;


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags panel_flags = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		std::shared_ptr<Frame> m_frame;
		std::shared_ptr<SceneView> m_sceneview;
		std::shared_ptr<InspectorPanel> m_inspectorpanel;
		std::shared_ptr<ViewPanel> m_viewpanel;
		std::shared_ptr<TabbedPanel> m_TabbedPanel;
		std::shared_ptr<AssetBrowserPanel> m_AssetBrowser;

		static bool p_open;
		static bool opt_fullscreen;
		static bool opt_padding;		
		int display_width, display_height;

		static bool showRenderConfig;
		static bool showAudioConfig;
		static bool showPhysicsConfig;
		static bool showInputConfig;
		static bool showProjectConfig;

		static void showRenderManager(bool* p_open, std::unique_ptr<Fracture::Renderer>& _renderer);
		static void showAudioManager(bool* p_open);
		static void showPhysicsManager(bool* p_open);
		static void showInputManager(bool* p_open);
		static void showProjectSettings(bool* p_open);
	};
}

#endif