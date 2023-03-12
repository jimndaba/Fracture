#include "FracturePCH.h"
#include "Texture.h"

Fracture::Texture::Texture(const TextureCreationInfo& info):IGraphicsResource(),Description(info)
{
}

Fracture::Texture::~Texture()
{
	glDeleteTextures(1, &Handle);
}
