#pragma once
#ifndef EDITOR_H
#define EDITOR_H
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL2/SDL.h"
#include "GLAD/glad.h"
#include <memory>

namespace Fracture
{
	class Game;
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
		void onUpdate();
		void onShutdown();

		void SetGame(Fracture::Game* game);

		void Render();

		bool done;

	private:
		Game* m_game;
		SDL_Window* window;
		SDL_GLContext gl_context;
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

		void ToolBar();
	};
}

#endif