#pragma once
#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <memory>
#include <string>

namespace Fracture
{
	class Texture2D;
	class TextureCubeMap;
	class TextureMultiSample;

	class TextureLoader
	{
	public:
		TextureLoader();

		static std::shared_ptr<Texture2D> LoadTexture2D(const std::string& name, const std::string& path);
		static std::shared_ptr<Texture2D> LoadTexture2D(const std::string& name, const std::string& filename, const std::string& directory);
		static std::shared_ptr<Texture2D> LoadHDR(const std::string& name, const std::string& path);
		static std::shared_ptr<TextureCubeMap> LoadCubeMap(const std::string& name, const std::string& path);
		static std::shared_ptr<TextureMultiSample> LoadTextureMultiSample(const std::string& name, const std::string& path);

	};
}

#endif