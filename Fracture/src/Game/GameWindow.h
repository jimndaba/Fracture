#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <iostream>

#include "GLAD/glad.h"

namespace Fracture
{
	class Game;
	class GameWindow
	{

	public:
		GameWindow();
		~GameWindow();

		void pollEvents(Game& game);
		void swapBuffers();

	private:
		SDL_Window* m_window;
		SDL_Event m_event;
		SDL_GLContext maincontext;
	};
}

#endif