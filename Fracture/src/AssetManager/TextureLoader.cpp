#include "TextureLoader.h"
#include "Logging/Logger.h"
#include "Rendering/OpenGL/Texture.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/Texture2DMultiSample.h"
#include "Rendering/OpenGL/TextureCubeMap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include "Rendering/OpenGL/OpenGLBase.h"

Fracture::TextureLoader::TextureLoader()
{
}

std::shared_ptr<Fracture::Texture2D> Fracture::TextureLoader::LoadTexture2D(const std::string& name, const std::string& path)
{
	int width;
	int height;
	int channel;
	std::shared_ptr<Fracture::Texture2D> texture;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 0);

	if (data)
	{

		TextureFormat format = TextureFormat::RGB;
		
		if (channel == 1)
			format = TextureFormat::Red;
		else if (channel == 3)
			format = TextureFormat::RGB;
		else if (channel == 4)
			format = TextureFormat::RGBA;

		texture = Texture2D::CreateTexture(data, InternalFormat::RGBA16, format, width, height, glWrap::ClampToEdge, FormatType::UByte);

		texture->GenerateMips();

		stbi_image_free(data);
	}
	else
	{
		FRACTURE_ERROR("Failed to load texture {}", path);
	}

	return texture;
}

std::shared_ptr<Fracture::Texture2D> Fracture::TextureLoader::LoadTexture2D(const std::string& name, const std::string& filename, const std::string& directory)
{
	std::string file = filename;
	std::string path = directory + '/' + file;

	auto texture = TextureLoader::LoadTexture2D(name, path);

	return texture;

}

std::shared_ptr<Fracture::Texture2D> Fracture::TextureLoader::LoadHDR(const std::string& name, const std::string& path)
{
	int width;
	int height;
	int channel;
	std::shared_ptr<Fracture::Texture2D> texture;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path.c_str(), &width, &height, &channel, 0);
	
	if (data)
	{
		texture = Texture2D::CreateTexture(data,InternalFormat::RGBA16, TextureFormat::RGB, width, height, glWrap::ClampToEdge, FormatType::Float);

		stbi_image_free(data);
	}
	else
	{
		FRACTURE_ERROR("Failed to load texture {}", path);
	}
	   
	return texture;
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::TextureLoader::LoadCubeMap(const std::string& name, const std::string& path)
{
	return std::shared_ptr<TextureCubeMap>();
}

std::shared_ptr<Fracture::TextureMultiSample> Fracture::TextureLoader::LoadTextureMultiSample(const std::string& name, const std::string& path)
{
	return std::shared_ptr<TextureMultiSample>();
}


