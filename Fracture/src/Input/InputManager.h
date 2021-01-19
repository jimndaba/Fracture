#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include "Keycode.h"
#include "Mousecode.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "MouseState.h"

namespace Fracture
{

	class InputManager
	{

	public:
		InputManager();
		~InputManager();

		static bool IsKeyDown(KeyCode key);


		static bool IsMouseDown(MOUSECODE key);
		static bool IsMouseUp(MOUSECODE key);
		static glm::vec2 GetMousePosition();
		static bool IsMouseScroll();
		static glm::vec2 GetMouseScroll();

		static bool GetMouseState();

		Mouse GetMouse();

		static void PollEvents();
		static SDL_Event event;
	private:
		static Mouse m_mouse;
	};

}

#endif