#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include "Keycode.h"
#include "Mousecode.h"
#include <glm/glm.hpp>
#include "glfw/glfw3.h"


namespace Fracture
{
	class Mouse;
	enum class KeyboardState
	{
		Pressed,
		Released,
		Down,
		Up,
		WheelUp,
		WheelDown,
	};

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

		static Mouse GetMouse();

		static void PollEvents();
	
	private:
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		static KeyboardState m_keyState;
		static Mouse m_mouse;

		static glm::vec2 m_scroll;
		static bool isScrolling;
	};

}

#endif