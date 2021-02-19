#pragma once
#ifndef OPENGLBASE_H
#define OPENGLBASE_H

#include <GLAD\glad.h>


namespace Fracture
{
	enum class InternalFormat
	{
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthStencil = GL_DEPTH_STENCIL,
		Depth = GL_DEPTH_COMPONENT,
		Red = GL_RED,
		RedGreen = GL_RG,
		RedGreen16 = GL_RG16F,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGBA16 = GL_RGBA16F,
		RGBA32 = GL_RGBA32F
	};

	enum class TextureFormat
	{
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL
	};

	enum class FormatType
	{
		UByte = GL_UNSIGNED_BYTE,
		UShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Half_Float = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		UInt24_8 = GL_UNSIGNED_INT_24_8
	};

	enum class glWrap
	{
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		CampToBorder = GL_CLAMP_TO_BORDER
	};

	enum class glAttachmentType
	{
		Color = GL_COLOR_ATTACHMENT0,
		Depth = GL_DEPTH_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	enum class glAttachmentTarget
	{
		Texture2D = GL_TEXTURE_2D,
		MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
		CubeMapPosX = GL_TEXTURE_CUBE_MAP_POSITIVE_X
	};
	
}

#endif