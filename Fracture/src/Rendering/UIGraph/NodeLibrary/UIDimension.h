#pragma once
#ifndef UIDIMENSION_H
#define UIDIMENSION_H

namespace Fracture
{

	class UIDimension
	{

	public:
		UIDimension() {};
		UIDimension(float _x, float _y, float _Width, float _Height)
		{
			X = _x;
			Y = _y;
			Width = _Width;
			Height = _Height;
		}

		float X = 0;
		float Y = 0;
		float Width = 0;
		float Height = 0;

	};


}

#endif