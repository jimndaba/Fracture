#pragma once
#ifndef MOUSECODE_H
#define MOUSECODE_H

#include "SDL2/SDL.h"

namespace Fracture
{

	enum class MOUSECODE
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2



	};


}

#endif