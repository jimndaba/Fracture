#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Fracture
{

	enum class TextureType
	{
		Diffuse,
		Specular,
		Normal,
		Height,
		Bump,
		Reflection,
	};

	class Texture
	{
	public:
		Texture();
		Texture(std::string name);
		Texture(std::string name,int Width, int Height);

		void Bind();
		void Unbind();


		std::string Name;
		std::string path;
		TextureType textureType;
		unsigned char* m_data;
		int width, height, channel = 0;
		unsigned int id;
		std::string type;

	};

}

#endif