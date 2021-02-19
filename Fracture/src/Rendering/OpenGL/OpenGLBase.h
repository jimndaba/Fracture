#pragma once
#include <GLAD\glad.h>
#ifndef OPENGLBASE_H
#define OPENGLBASE_H

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
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};

	enum class TextureFormat
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		Float16 = GL_RGBA16F,
		Float32 = GL_RGBA32F
	};

	enum class glFormatType
	{



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