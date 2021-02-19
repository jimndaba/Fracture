#pragma once
#ifndef TEXTURECUBEMAP_H
#define TEXTURECUBEMAP_H

#include "Texture.h"

namespace Fracture
{

	class TextureCubeMap : public Texture
	{
	public:
		TextureCubeMap();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetMipLevel() = 0;

		virtual uint32_t GetTextureID() = 0;
	};



}

#endif