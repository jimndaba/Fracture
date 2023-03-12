#pragma once
#ifndef EVENT_H
#define EVENT_H


namespace Fracture
{
	struct Event
	{
		virtual const char* Name() = 0;
	};

}
#endif