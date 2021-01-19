#pragma once
#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include "IInputState.h"
#include "Mousecode.h"
#include "glm/glm.hpp"

namespace Fracture
{
	enum class MouseState
	{
		Pressed,
		Released,
		Down,
		Up,
		WheelUp,
		WheelDown,
	};

	class Mouse: public InputState
	{

	public:
		Mouse();
		~Mouse();
		static bool IsButtonDown(MOUSECODE key);
		static bool IsButtonUp(MOUSECODE key);
		static glm::vec2 GetPosition();
		static bool IsScrolling();
		static glm::vec2 GetScroll();

		static MouseState GetState();

	private:
		static MouseState m_MouseState;
		static SDL_Event event;
	};

}

#endif