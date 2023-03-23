#include "FracturePCH.h"
#include "ImagerLoader.h"
#include "Rendering/Texture.h"
#include "Serialisation/Serialiser.h"

Fracture::ImageLoader::ImageLoader()
{
}

std::shared_ptr<Fracture::Texture> Fracture::ImageLoader::LoadTexture(const std::string& path)
{
	FILE* f = fopen(path.c_str(), "rb");

	assert(f);//File Not converted or present
	if (!f)
	{
		FRACTURE_ERROR("Cannot open {}. Did you forget to imported texture", path);
		return nullptr;
	}



	TextureFileHeader header;
	if (fread(&header, 1, sizeof(header), f) != sizeof(header))
	{
		FRACTURE_ERROR("Unable to read texture file header: {}", path);
		return nullptr;
	}

	TextureCreationInfo info;

	std::vector<uint8_t> data;
	data.resize(header.Count);
	

	if (fread(data.data(), sizeof(uint8_t), header.Count, f) != header.Count)
	{
		FRACTURE_ERROR("Could not read Pixel Data descriptors");
		return nullptr;
	}

	fclose(f);
	info.data = data;
	info.Width = header.Width;
	info.Height = header.Height;

	switch (header.Channels)
	{
		case 1:
		{
			info.format = TextureFormat::Red;
			info.internalFormat = InternalFormat::R;
			break;
		}
		case 2:
		{
			info.format = TextureFormat::RG;
			info.internalFormat = InternalFormat::RG16;
			break;
		}
		case 3:
		{
			info.format = TextureFormat::RGB;
			info.internalFormat = InternalFormat::RGB8;
			break;
		}
		case 4:
		{
			info.format = TextureFormat::RGBA;
			info.internalFormat = InternalFormat::RGBA16;
			break;
		}
	}

	std::shared_ptr<Fracture::Texture> texture = std::make_shared<Fracture::Texture>(info);
    return texture;
}
