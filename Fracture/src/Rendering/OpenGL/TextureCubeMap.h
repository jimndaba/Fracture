#pragma once
#ifndef TEXTURECUBEMAP_H
#define TEXTURECUBEMAP_H

#include "Texture.h"

namespace Fracture
{

	class TextureCubeMap : public Texture
	{
	public:
		TextureCubeMap(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType, bool GenMips);
		TextureCubeMap(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType, bool GenMips);

		void bind()const;
		void unbind()const;
		void GenerateMips();

		uint32_t TextureUnit()const;
		uint32_t GetWidth()const;
		uint32_t GetHeight()const;
		uint32_t GetMipLevel()const;

		void resize(int width, int height)const;
		uint32_t GetTextureID()const;

		static std::shared_ptr<TextureCubeMap> CreateTexture(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType, bool GenMips);
		static std::shared_ptr<TextureCubeMap> CreateTexture(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType, bool GenMips);

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