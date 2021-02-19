#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>

namespace Fracture
{	

	enum class InternalFormat;
	enum class TextureFormat;
	enum class glType;
	enum class glWrap;
	enum class FormatType;

	class Texture
	{
	public:
		~Texture() {}

		virtual void bind()const = 0;
		virtual void unbind()const = 0;

		virtual void Resize(uint32_t width,uint32_t height) {};

		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual uint32_t GetMipLevel()const = 0;

		virtual uint32_t GetTextureID()const = 0;
	};



}

#endif