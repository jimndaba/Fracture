#pragma once
#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H
#include "EventSystem/Event.h"

namespace Fracture
{

	struct KeyboardEvent : public Event
	{
		KeyboardEvent(uint8_t _key, uint8_t _scancode, uint8_t _action, uint8_t _mods) :Event(),
			key(_key),scancode(_scancode),action(_action),mods(mods)
		{};

		uint8_t key;
		uint8_t scancode;
		uint8_t action;
		uint8_t mods;

		const char* Name() { return "KeybaordEvent"; };
	};

	struct MouseMoveEvent : public Event
	{
		MouseMoveEvent(float xpos, float ypos) :Event(),
			XPos(xpos), YPos(ypos)
		{};

		float XPos;
		float YPos;
		const char* Name() { return "KeybaordEvent"; };
	};

	struct MouseButtonEvent : public Event
	{
		MouseButtonEvent(int button, int action, int mods) :Event(),
			Button(button),Action(action),Mods(mods)
		{};

		uint8_t Button;
		uint8_t Action;
		uint8_t Mods;
		const char* Name() { return "KeybaordEvent"; };
	};

	struct GamepadEvent : public Event
	{
		GamepadEvent(uint8_t _key, uint8_t _scancode, uint8_t _action, uint8_t _mods) :Event(),
			key(_key), scancode(_scancode), action(_action), mods(mods)
		{};

		uint8_t key;
		uint8_t scancode;
		uint8_t action;
		uint8_t mods;

		const char* Name() { return "KeybaordEvent"; };
	};

	struct GamepadConnectEvent : public Event
	{
		GamepadConnectEvent() :Event()
		{}
		const char* Name() { return "GamepadConnectedEvent"; };
	};

	struct GamepadDisconnectedEvent : public Event
	{
		GamepadDisconnectedEvent():Event()
		{
		}
		const char* Name() { return "GamepadDisconnectedEvent"; };
	};

	struct SaveBidingsEvent : public Event
	{
		SaveBidingsEvent() :Event()
		{
		}
		const char* Name() { return "SaveBidingsEvent"; };
	};

}

#endif