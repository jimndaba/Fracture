#pragma once
#ifndef GAMEPAD_H
#define GAMEPAD_H

namespace Fracture
{
	enum class GamepadType : int32_t {
		Other,
		DualShock3 = 616,
		DualShock4 = 1476,
		DualShock4Slim = 2508,
		XBOX360 = 654,
		JoyconL = 8198,
		JoyconR = 8199,
		ProController = 8201,
	};

    enum class JoystickButton : int32_t {
        RightDown = GLFW_GAMEPAD_BUTTON_A,
        RightRight = GLFW_GAMEPAD_BUTTON_B,
        RightLeft = GLFW_GAMEPAD_BUTTON_X,
        RightUp = GLFW_GAMEPAD_BUTTON_Y,
        LeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
        RightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
        Back = GLFW_GAMEPAD_BUTTON_BACK,
        Start = GLFW_GAMEPAD_BUTTON_START,
        Guide = GLFW_GAMEPAD_BUTTON_GUIDE,
        LeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
        RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
        DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
        DPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
        DPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
        DPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
    };

    enum class JoystickAxis : int32_t {
        LeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
        LeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
        RightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
        RightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
        LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
        RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    };

	struct Gamepad
	{
		GamepadType Gamepadtype;
		bool Button_A = false;
		bool Button_B = false;
		glm::vec2 LeftStick_Axis = glm::vec2(0.0f);
		glm::vec2 RightStick_Axis = glm::vec2(0.0f);
	};






}


#endif