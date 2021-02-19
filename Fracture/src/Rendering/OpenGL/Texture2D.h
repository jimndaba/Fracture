#pragma once
#ifndef TEXTUREMULTISAMPLE_H
#define TEXTUREMULTISAMPLE_H

#include "Texture.h"

namespace Fracture
{

	class Texture2D : public Texture
	{
	public:
		Texture2D(TextureFormat format, uint32_t width, uint32_t height, glWrap wrap);
		Texture2D(void* data, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap);

		void bind()const;
		void unbind()const;

		uint32_t GetWidth()const;
		uint32_t GetHeight()const;
		uint32_t GetMipLevel()const;

		void resize(int width, int height)const;
		uint32_t GetTextureID()const;

		static std::shared_ptr<Texture2D> CreateTexture(TextureFormat format, uint32_t width, uint32_t height, glWrap wrap);
		static std::shared_ptr<Texture2D> CreateTexture(void* data, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap);

		

	private:
		uint32_t m_TextureID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_MipLevels;

		TextureFormat m_Format;
		glWrap m_TextureWrap;
	};



}

#endif