#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GLAD/glad.h>

namespace Fracture
{

	enum class TextureType
	{
		Diffuse= 3,
		Normal,
		Metallic,
		Roughness,
		AO,
		Specular,		
		Height,	
		Bump,
		Reflection,
		ColorAttachment,
		DepthStencilAttachment,
		Environment,
		CubeMap
	};

	enum class TextureTarget
	{
		Texture2D,
		MultiSample,
		CubeMap,
	};

	class Texture
	{
	public:
		Texture(Fracture::TextureType mtype);
		Texture(std::string name, Fracture::TextureType mtype);
		Texture(std::string name,int Width, int Height, Fracture::TextureType mtype);
		Texture(std::string name, int Width, int Height, Fracture::TextureType mtype, void* data);
		Texture(std::string name, int Width, int Height, TextureTarget TextureTarget ,GLenum internalFormat, GLenum format, GLenum type, Fracture::TextureType mtype);
		Texture(std::string name, int Width, int Height, GLenum internalFormat, GLenum format, GLenum type, Fracture::TextureType mtype);
	

		void Bind();
		void Unbind();

		void Resize(int width,int height);


		std::string Name;
		std::string path;
		TextureType textureType;
		GLenum      Type;
		GLenum		InternalFormat;
		GLenum		Format;
		TextureTarget mTexturetarget;
		unsigned char* m_data;
		int width, height, channel = 0;
		unsigned int id;
		std::string type;



	};

}

#endif