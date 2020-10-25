#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "GLAD/glad.h"

namespace Fracture
{
	class Game;
	class GameWindow
	{

	public:
		GameWindow(int width, int height,std::string title);
		~GameWindow();

		void pollEvents(Game& game);
		void swapBuffers();

		int Width;
		int Height;
		std::string Title;

		SDL_Window* Context()
		{
			return m_window;
		}

		SDL_GLContext glContext()
		{
			return maincontext;
		}

	private:
		SDL_Window* m_window;
		SDL_Event m_event;
		SDL_GLContext maincontext;
	};
}

#endif