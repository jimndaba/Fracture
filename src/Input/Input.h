#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "InputEvents.h"
#include "GLFW/glfw3.h"
#include <map>


namespace Fracture
{
	struct InputSource
	{
		enum class SourceType : uint8_t
		{
			Keyboard,
			Mouse,
			Gamepad0,
			Gamepad1,
			Gamepad2,
			Gamepad3,
			Gamepad4,
		};

		enum class State
		{
			Up,
			Down,
			Pressed,
			Released,
		};

		float Multiplier = 0.0f;
		uint8_t scanCode;
		uint8_t key;
		State InputState = State::Up;
		bool IsRepeating;
		SourceType Device;

		float FLOAT = 0.0f;
		glm::vec2 VEC2 = glm::vec2(0);
	};

	struct InputBinding
	{
		enum class Type
		{
			Button, 
			Axis
		};				
		Type BindingType = Type::Button;
		std::vector<std::shared_ptr<InputSource>> Sources;
	};

	class Input
	{
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void Joystick_callback(int jid, int event);

		static glm::vec2 m_scroll;

		static std::map<std::string, KeyCode> mKeyMapping;

		int NoConnectedGamepads = 0;
	

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

		void OnKeyboardEvent(const std::shared_ptr<KeyboardEvent>& evnt);
		void OnGamepadConnectedEvent(const std::shared_ptr<GamepadConnectEvent>& evnt);
		void OnGamepadDisconnectedEvent(const std::shared_ptr<GamepadDisconnectedEvent>& evnt);

		void OnSaveBingdingsEvent(const std::shared_ptr<SaveBidingsEvent>& evnt);

		static glm::vec2 GetMousePosition();		
		static glm::vec2 GetMouseScroll();

		static void RegisterKeyMap(const std::string& Name, KeyCode key);
		static void RemoveKeyMap(const std::string& Name);

		static bool ButtonPressed(const std::string& Name);
		static bool ButtonReleased(const std::string& Name);
		static float GetAxis(const std::string& Name);


		std::map<Fracture::UUID, std::shared_ptr<InputSource>> InputSources;
		static std::unordered_map<std::string, InputBinding> InputBindings;

		std::map<Fracture::UUID, std::string> InputSorucesNames;

		void BindInput(const std::string& action);
		void BindInput(const std::string& action,const std::shared_ptr<InputSource>& source, InputBinding::Type bindingType = InputBinding::Type::Button);




	};





}


#endif