#pragma once
#ifndef MOUSECODE_H
#define MOUSECODE_H

#include "SDL2/SDL.h"

namespace Fracture
{

	enum class MOUSECODE : Uint32
	{
		// From glfw3.h
		ButtonLeft = SDL_BUTTON_LEFT,
		ButtonRight = SDL_BUTTON_RIGHT,
		ButtonMiddle = SDL_BUTTON_MIDDLE
	};


}

#endif