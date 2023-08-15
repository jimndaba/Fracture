#pragma once
#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "AssetRegistries.h"

namespace Fracture
{
	struct Texture;

	class ImageLoader
	{
	public:
		ImageLoader();
		
		static std::shared_ptr<Texture> LoadTexture(const std::string& path);


		static void SaveImage(std::string filepath, int width, int height, int nrChannels, std::vector<char> buffer, uint32_t stride);

	};

	struct ImageImporter
	{
		ImageImporter();
		static Fracture::TextureRegistry LoadTexture(const std::string& path, const std::string& out_path = "Content\\textures\\");
		static Fracture::TextureRegistry LoadHDRTexture(const std::string& path, const std::string& out_path = "Content\\textures\\");
		static void FreeData(const void* data);
		static bool copyFile(const char* SRC, const char* DEST);
	};



}


#endif