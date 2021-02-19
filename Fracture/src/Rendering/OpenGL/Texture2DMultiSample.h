#pragma once
#ifndef TEXTUREMULTISAMPLE_H
#define TEXTUREMULTISAMPLE_H

#include "Texture.h"

namespace Fracture
{

	class Texture2DMultiSample : public Texture
	{
	public:
		Texture2DMultiSample();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetMipLevel() = 0;


		virtual void resize(int width, int height) = 0;
		virtual uint32_t GetTextureID() = 0;
	};



}

#endif