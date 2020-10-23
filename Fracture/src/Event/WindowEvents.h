#pragma once
#ifndef WINDOWEVENTS_H
#define WINDOWEVENTS_H

#include "Event.h"

namespace Fracture
{
	struct WindowResizeEvent : public Event
	{
		WindowResizeEvent(int w, int h)
		{
			Width = w;
			Height = h;
		}

		int Width;
		int Height;

		virtual void Handled() {}

	};


	
}

#endif