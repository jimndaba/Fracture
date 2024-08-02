#include "FracturePCH.h"
#include "ImagerLoader.h"
#include "Rendering/Texture.h"
#include "Serialisation/Serialiser.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


Fracture::ImageLoader::ImageLoader()
{
}

std::shared_ptr<Fracture::Texture> Fracture::ImageLoader::LoadTexture(const std::string& path, int texture_type)
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

	std::vector<unsigned char> u_data;
	std::vector<float> f_data;
	if (header.magicValue == 0 || texture_type == 0)
	{
		u_data.resize(header.Count);
		if (fread(u_data.data(), sizeof(unsigned char), header.Count, f) != header.Count)
		{
			FRACTURE_ERROR("Could not read Pixel Data descriptors");
			return nullptr;
		}
		info.data = u_data;
		info.Texture_Type = TextureCreationInfo::TextureType::Texture2D;
		info.formatType = TextureFormatType::UByte;
		info.Wrap = TextureWrap::Repeat;
	}
	
	if (header.magicValue == 1)
	{
		f_data.resize(header.Count);
		fread(f_data.data(), sizeof(float), header.Count, f);
		if (f_data.size() != header.Count)
		{
			FRACTURE_ERROR("Could not read Pixel Data descriptors");
			return nullptr;
		}
		info.f_data = f_data;
		info.Texture_Type = TextureCreationInfo::TextureType::HDR;
		info.formatType = TextureFormatType::Float;
		info.Wrap = TextureWrap::ClampToEdge;
	}

	fclose(f);	
	info.Width = header.Width;
	info.Height = header.Height;

	switch (header.Channels)
	{
		case 1:
		{
			info.format = TextureFormat::Red;
			info.internalFormat = InternalFormat::R16F;
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
			info.internalFormat = InternalFormat::RGB16F;
			break;
		}
		case 4:
		{
			info.format = TextureFormat::RGBA;
			info.internalFormat = InternalFormat::RGBA16F;
			break;
		}
	}	
	
	std::shared_ptr<Fracture::Texture> texture = std::make_shared<Fracture::Texture>(info);

    return texture;
}

void Fracture::ImageLoader::SaveImage(std::string filepath, int width, int height, int nrChannels, std::vector<char> buffer, uint32_t stride)
{
	stbi_flip_vertically_on_write(true);
	if (stbi_write_png(filepath.c_str(), width, height, nrChannels, buffer.data(), stride))
	{
		
		FRACTURE_INFO("Saved image");
	}
	else
	{
		FRACTURE_ERROR("Failed to save screenshot");
	}
}

Fracture::ImageImporter::ImageImporter()
{
}

Fracture::TextureRegistry Fracture::ImageImporter::LoadTexture(const std::string& path, const std::string& out_path)
{
	stbi_set_flip_vertically_on_load(true);
	int width;
	int height;
	int channels;	
	std::vector<uint8_t> mdata; //unsigned char* data

	std::string file_name = path.substr(path.find_last_of("/\\") + 1);
	std::string::size_type const p(file_name.find_last_of('.'));
	std::string file_without_extension = file_name.substr(0, p);

	FRACTURE_TRACE("Importing Image {}", path);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		mdata.assign(data, data + ((int64_t)width * (int64_t)height * channels));

		std::string Output_path = out_path;
		FILE* f = fopen(Output_path.c_str(), "wb");

		if (!f)
		{
			FRACTURE_ERROR("Failed to save Texture: {}", Output_path);
			Fracture::TextureRegistry reg;
			reg.Success = false;
			return reg;
		}


		Fracture::TextureFileHeader header;
		header.magicValue = 0;
		header.ID = Fracture::UUID();
		header.Width = width;
		header.Height = height;
		header.Channels = channels;
		header.Count = mdata.size();
		header.DataSize = ((int64_t)width * (int64_t)height * channels);

		fwrite(&header, sizeof(header), 1, f);
		fwrite(&mdata[0], sizeof(mdata[0]), mdata.size(), f);
		fclose(f);

		FRACTURE_INFO("Imported Texture: {}", path);
		FreeData(data);

		Fracture::TextureRegistry reg;
		reg.ID = Fracture::UUID();
		reg.Name = file_without_extension;
		reg.Path = Output_path;
		reg.Success = true;
		return reg;
	}

	Fracture::TextureRegistry reg;
	reg.Success = false;
	return reg;

}

Fracture::TextureRegistry Fracture::ImageImporter::LoadHDRTexture(const std::string& path, const std::string& out_path)
{
	
	
	int width;
	int height;
	int channels;	
	std::vector<float> mdata;

	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);

	mdata.assign(data, data + ((int64_t)width * (int64_t)height * channels));

	if (data)
	{
		std::string file_name = path.substr(path.find_last_of("/\\") + 1);
		std::string::size_type const p(file_name.find_last_of('.'));
		std::string file_without_extension = file_name.substr(0, p);	

		std::string Output_path = out_path;
		FILE* f = fopen(Output_path.c_str(), "wb");
		Fracture::TextureFileHeader header;
		header.magicValue = 1;
		header.ID = Fracture::UUID();
		header.Width = width;
		header.Height = height;
		header.Channels = channels;
		header.Count = mdata.size();
		header.DataSize = ((int64_t)width * (int64_t)height * channels);
		fwrite(&header, sizeof(header), 1, f);
		fwrite(mdata.data(), sizeof(mdata[0]), mdata.size(), f);
		fclose(f);

		FRACTURE_INFO("Imported HDR Texture: {}", path);		
		FreeData(data);


		Fracture::TextureRegistry reg;
		reg.ID = Fracture::UUID();
		reg.Name = file_without_extension;
		reg.Path = Output_path;
		return reg;
	}
}

void Fracture::ImageImporter::FreeData(const void* data)
{
	stbi_image_free((void*)data);
}

// copy in binary mode
bool Fracture::ImageImporter::copyFile(const char* SRC, const char* DEST)
{
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	return src && dest;
}