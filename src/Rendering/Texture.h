#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "IGraphicsResource.h"

namespace Fracture
{
	enum class TextureTarget
	{
		Texture2D = GL_TEXTURE_2D,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		Texture3D = GL_TEXTURE_3D,
		TextureCubeMap = GL_TEXTURE_CUBE_MAP,
		TextureCubeMapPOSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		TextureCubeMapNEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		TextureCubeMapPOSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		TextureCubeMapNEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TextureCubeMapPOSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		TextureCubeMapNEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		Texture2DMultiSamples = GL_TEXTURE_2D_MULTISAMPLE,
		Texture2DMultiSamplesArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class AttachmentTarget
	{
		None = GL_NONE,
		Color = GL_COLOR_ATTACHMENT0,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	enum class TextureWrap
	{
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		CampToBorder = GL_CLAMP_TO_BORDER
	};

	enum class InternalFormat
	{
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		Depth32Stencil8 = GL_DEPTH32F_STENCIL8,
		DepthComponent32f = GL_DEPTH_COMPONENT32F,
		DepthComponent32 = GL_DEPTH_COMPONENT32,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthStencil = GL_DEPTH_STENCIL,
		Depth = GL_DEPTH,
		R = GL_R,
		RG = GL_RG,
		RG16 = GL_RG16,
		RGB = GL_RGB,
		RGB8 = GL_RGB8,
		RGB16 = GL_RGB16,
		RGB16F = GL_RGB16F,
		RGBA = GL_RGBA,
		RGBA16 = GL_RGBA16,
		RGBA16F = GL_RGB16F,
		RGBA32F = GL_RGBA32F,
		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,
		SRGBA = GL_SRGB_ALPHA
	};

	enum class TextureFormat
	{
		Red = GL_R,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		SRGB = GL_SRGB,
		SRGBA = GL_SRGB_ALPHA,
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
	};

	enum class TextureMinFilter
	{
		Near = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class TextureMagFilter
	{
		Near = GL_NEAREST,
		Linear = GL_LINEAR,
	};

	enum class TextureFormatType
	{
		RedFloat = GL_R,
		UByte = GL_UNSIGNED_BYTE,
		UShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Half_Float = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		UInt24_8 = GL_UNSIGNED_INT_24_8,
		Float32U248 = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
	};


	struct TextureCreationInfo
	{
		UUID ID = UUID(0, false);
		std::string Name;
		std::string Path;		
		TextureTarget TextureTarget = TextureTarget::Texture2D;
		AttachmentTarget AttachmentTrgt = AttachmentTarget::Color;
		InternalFormat internalFormat = InternalFormat::RGB8;
		TextureFormat format = TextureFormat::RGB;
		TextureFormatType formatType = TextureFormatType::UByte;
		TextureMagFilter magFilter = TextureMagFilter::Linear;
		TextureMinFilter minFilter = TextureMinFilter::Linear;
		TextureWrap Wrap = TextureWrap::ClampToEdge;
		int Width = 1;
		int Height = 1;
		int NoChannels = 1;
		int Depth = 1;
		uint32_t MipLevels = 1;
		int TextureArrayLevels = 1;
		bool GenMinMaps = false;
		void* data = nullptr;

		uint32_t CaclMipLevels() const
		{
			return static_cast<uint32_t>(std::floor(std::log2(std::max(Width, Height)))) + 1;
		}
	};

	struct Texture : public IGraphicsResource
	{
		Texture(const TextureCreationInfo& info);
		~Texture();
		TextureCreationInfo Description;		
		uint32_t Handle = 0;

	private:
		// pure virtual implementation
		void bind() {};
	};



}

#endif