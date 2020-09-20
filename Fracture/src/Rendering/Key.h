#pragma once
#ifndef KEY_H
#define KEY_H

namespace Fracture
{
	enum class Transparency
	{
		Opaque = 0,
		Transparent = 1,
	};

	enum class ShadowDraw
	{
		NoShadows = 0,
		Shadows = 1,
	};



	struct Key
	{
		Transparency transparency;
		ShadowDraw shadowDraw;
	};

}


#endif 