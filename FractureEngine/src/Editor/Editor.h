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

	private:
		SDL_Window* window;
		SDL_GLContext gl_context;

		int display_width, display_height;
	};
}

#endif