#pragma once
#ifndef EVENT_H
#define EVENT_H

namespace Fracture
{
	
	struct Event
	{
		virtual void Handled() = 0;
	};


	
}

#endif