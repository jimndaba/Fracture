#pragma once
#ifndef EDITOR_H
#define EDITOR_H
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL2/SDL.h"
#include "GLAD/glad.h"

namespace Fracture
{

	class Editor
	{
	public:
		Editor();
		~Editor();

		void onInit();
		void onUpdate();

		void Begin();
		void Render();
		void End();

	private:
		SDL_Window* window;
		SDL_GLContext gl_context;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags panel_flags = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		bool done;
		static bool p_open;
		static bool opt_fullscreen;
		static bool opt_padding;
		
		int display_width, display_height;

		void ToolBar();
	};
}

#endif