#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"
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
	struct ProjectProperties;
	struct GameSettings;
	class EditorCamera;
	class FreeCamera;
	class EditorFrameGraph;
	class AnimationManager;
	class AnimatorProbe;
	struct Event;

	class Editor
	{
	public:
		Editor();
		~Editor();

		void onInit();
		bool onLoad();
		void onLoadNew();
		void run();
		void onUpdate(float dt);
		void onRender();
		void onShutdown();
		void Render();
		void SetScene();

		static void oEvent(Event* m_event);

		static void onChangeTitleName(std::string title);

		bool done;

		void DrawMenuBar();

		static void isNewProject(bool isnew);

		static std::shared_ptr<Scene> ActiveScene();


		std::shared_ptr<Logger> GetLogger();
		std::shared_ptr<ProjectProperties> Properties();

		static std::shared_ptr<EditorFrameGraph> m_graph; //TODO rename or hide
	private:

	
		bool frameLimiter = true;
		double time = 0.0;
		const double dt = 0.01;
		double currentTime;
		double accumulator = 0.0;

		std::shared_ptr<Logger> m_logger;
		std::unique_ptr<Profiler> m_Profiler;
		std::shared_ptr<Renderer> m_Renderer;
		std::shared_ptr<GameWindow> m_window;
		static std::unique_ptr<Eventbus> m_Eventbus;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<AnimationManager> m_AnimationManger;
		
		std::unique_ptr<PhysicsManager> m_PhysicsManger;
		std::unique_ptr<AssetManager> m_AssetManger;
		std::shared_ptr<ScriptManager> m_ScriptManger;
		static std::unique_ptr<SceneManager> m_SceneManager;
		static std::unique_ptr<EntityFactory> m_EntityFactory;
		static std::shared_ptr<Scene> m_ActiveScene;
		//std::shared_ptr<EditorCamera> camera;
		std::shared_ptr<FreeCamera> camera;

		std::shared_ptr<Texture> m_irradiance;
		std::shared_ptr<Texture> m_prefilter;
		

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags panel_flags = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		std::shared_ptr<Frame> m_frame;
		static std::shared_ptr<SceneView> m_sceneview;
		std::shared_ptr<InspectorPanel> m_inspectorpanel;
		std::shared_ptr<ViewPanel> m_viewpanel;
		std::shared_ptr<TabbedPanel> m_TabbedPanel;
		std::shared_ptr<AssetBrowserPanel> m_AssetBrowser;
		static std::shared_ptr<ProjectProperties> m_properties;
		static std::shared_ptr <GameSettings> m_GameSettings;

		static bool m_loadNewProject;
		static bool p_open;
		static bool opt_fullscreen;
		static bool opt_padding;		
		int display_width, display_height;

		static bool showRenderConfig;
		static bool showAudioConfig;
		static bool showPhysicsConfig;
		static bool showInputConfig;
		static bool showProjectConfig;

		// view meny 
		static bool showInspector;
		static bool showLogger;
		static bool showScenegraph ;
		static bool showAssets;
		static bool showViewport;

		static void showRenderManager(bool* p_open, std::shared_ptr<Fracture::Renderer>& _renderer);
		static void showAudioManager(bool* p_open);
		static void showPhysicsManager(bool* p_open);
		static void showInputManager(bool* p_open);
		static void showProjectSettings(bool* p_open,std::shared_ptr<Fracture::ProjectProperties>& m_properties);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}

#endif