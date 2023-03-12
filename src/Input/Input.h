#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

#include "GLFW/glfw3.h"
#include <map>


namespace Fracture
{
	//struct GLFWwindow;

	class Input
	{
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

		static glm::vec2 m_scroll;

		static std::map<std::string, KeyCode> mKeyMapping;

	public:
		Input();

		void Update();

		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool KeyPressed(KeyCode key);
		static bool KeyReleased(KeyCode key);


		static bool IsMouseDown(MouseCode btn);
		static bool IsMouseUp(MouseCode btn);

		static bool MousePressed(MouseCode btn);
		static bool MouseReleased(MouseCode btn);
		
		static bool IsMouseScrolling();

		static glm::vec2 GetMousePosition();		
		static glm::vec2 GetMouseScroll();

		static void RegisterKeyMap(const std::string& Name, KeyCode key);
		static void RemoveKeyMap(const std::string& Name);

		static bool ButtonPressed(const std::string& Name);
		static bool ButtonReleased(const std::string& Name);



	};





}


#endif