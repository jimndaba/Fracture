#pragma once
#ifndef IUPDATABLE_H
#define IUPDATABLE_H

namespace Fracture
{

	class IUPDATABLE
	{
	public:
		virtual void onUpdate(float dt) = 0;
	};

}

#endif

