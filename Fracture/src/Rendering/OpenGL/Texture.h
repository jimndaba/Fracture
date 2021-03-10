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

	enum class TextureType
	{
		Albedo = 3,
		Diffuse = 3,
		Normal,
		Roughness,
		Metallic,
		AO,
		Specular,
		Emission,
		IrradianceMap,
		PreFilterMap,
		Brdf,
		DirShadowMap,
		Grab,
		Depth,
	};

	class Texture
	{
	public:
		~Texture() {}

		virtual void bind()const = 0;
		virtual void unbind()const = 0;

		virtual void Resize(uint32_t width,uint32_t height) {};

		virtual uint32_t TextureUnit()const = 0;
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual uint32_t GetMipLevel()const = 0;
		virtual uint32_t GetTextureID()const = 0;

		TextureType GetType() const
		{
			return m_TextureType;
		}
		void SetType(const TextureType& textureType)
		{
			m_TextureType = textureType;
		}

		std::string GetName()const
		{
			return m_Name;
		}
		void SetName(const std::string Name)
		{
			m_Name = Name;
		}

		std::string GetPath()const
		{
			return m_Path;
		}

		void SetPath(const std::string path)
		{
			m_Path = path;
		}

	private:
		TextureType m_TextureType;
		std::string m_Name;
		std::string m_Path;
	};



}

#endif