#pragma once
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"

namespace Fracture
{

	class Texture2D : public Texture
	{
	public:
		Texture2D(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType m_FormatType);
		Texture2D(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType m_FormatType);

		~Texture2D() {};

		void bind()const;
		void unbind()const;
		void GenerateMips();

		uint32_t TextureUnit()const;
		uint32_t GetWidth()const;
		uint32_t GetHeight()const;
		uint32_t GetMipLevel()const;
		uint32_t GetTextureID()const;

		void resize(int width, int height)const;
	

		static std::shared_ptr<Texture2D> CreateTexture(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType);
		static std::shared_ptr<Texture2D> CreateTexture(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType);
		
	private:
		uint32_t m_TextureID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_MipLevels;

		void* m_data;

		InternalFormat m_InternalFormat;
		TextureFormat m_Format;
		FormatType m_FormatType;
		glWrap m_TextureWrap;
	};



}

#endif