#pragma once
#ifndef TEXTUREMULTISAMPLE_H
#define TEXTUREMULTISAMPLE_H

#include "Texture.h"

namespace Fracture
{

	class Texture2DMultiSample : public Texture
	{
	public:
		Texture2DMultiSample(InternalFormat internalformat, uint32_t noSamples, uint32_t width, uint32_t height);

		void bind()const;
		void unbind()const;
		void GenerateMips();

		uint32_t TextureUnit()const;
		uint32_t GetWidth()const;
		uint32_t GetHeight()const;
		uint32_t GetMipLevel()const;

		void resize(int width, int height)const;
		uint32_t GetTextureID()const;

		static std::shared_ptr<Texture2DMultiSample> CreateTexture(InternalFormat internalformat, uint32_t noSamples, uint32_t width, uint32_t height);

	private:
		uint32_t m_TextureID;
		uint32_t m_NoSamples;
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