#pragma once

#include <SDL2/SDL.h>

namespace Fracture
{
	enum class KeyCode
	{
		// From glfw3.h
		Space = SDL_SCANCODE_SPACE,
		Apostrophe = SDL_SCANCODE_APOSTROPHE, /* ' */
		Comma = SDL_SCANCODE_COMMA, /* , */
		Minus = SDL_SCANCODE_MINUS, /* - */
		Period = SDL_SCANCODE_PERIOD, /* . */
		Slash = SDL_SCANCODE_SLASH, /* / */

		D0 = SDL_SCANCODE_0, /* 0 */
		D1 = SDL_SCANCODE_1, /* 1 */
		D2 = SDL_SCANCODE_2, /* 2 */
		D3 = SDL_SCANCODE_3, /* 3 */
		D4 = SDL_SCANCODE_4, /* 4 */
		D5 = SDL_SCANCODE_5, /* 5 */
		D6 = SDL_SCANCODE_6, /* 6 */
		D7 = SDL_SCANCODE_7, /* 7 */
		D8 = SDL_SCANCODE_8, /* 8 */
		D9 = SDL_SCANCODE_9, /* 9 */

		Semicolon = SDL_SCANCODE_SEMICOLON, /* ; */
		Equal = SDL_SCANCODE_EQUALS, /* = */

		A = SDL_SCANCODE_A,
		B = SDL_SCANCODE_B,
		C = SDL_SCANCODE_C,
		D = SDL_SCANCODE_D,
		E = SDL_SCANCODE_E,
		F = SDL_SCANCODE_F,
		G = SDL_SCANCODE_G,
		H = SDL_SCANCODE_H,
		I = SDL_SCANCODE_I,
		J = SDL_SCANCODE_J,
		K = SDL_SCANCODE_K,
		L = SDL_SCANCODE_L,
		M = SDL_SCANCODE_M,
		N = SDL_SCANCODE_N,
		O = SDL_SCANCODE_O,
		P = SDL_SCANCODE_P,
		Q = SDL_SCANCODE_Q,
		R = SDL_SCANCODE_R,
		S = SDL_SCANCODE_S,
		T = SDL_SCANCODE_T,
		U = SDL_SCANCODE_U,
		V = SDL_SCANCODE_V,
		W = SDL_SCANCODE_W,
		X = SDL_SCANCODE_X,
		Y = SDL_SCANCODE_Y,
		Z = SDL_SCANCODE_Z,

		LeftBracket = SDL_SCANCODE_LEFTBRACKET,  /* [ */
		Backslash = SDL_SCANCODE_BACKSLASH,  /* \ */
		RightBracket = SDL_SCANCODE_RIGHTBRACKET,  /* ] */
		GraveAccent = SDL_SCANCODE_GRAVE,  /* ` */

		/* Function keys */
		Escape = SDL_SCANCODE_ESCAPE,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348


	};

}