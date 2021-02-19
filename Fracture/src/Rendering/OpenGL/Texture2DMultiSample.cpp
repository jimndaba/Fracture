#include "Texture2DMultiSample.h"

Fracture::Texture2DMultiSample::Texture2DMultiSample(InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType):Texture()
{
}

Fracture::Texture2DMultiSample::Texture2DMultiSample(InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType):Texture()
{
}

Fracture::Texture2DMultiSample::Texture2DMultiSample(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
}

void Fracture::Texture2DMultiSample::bind() const
{
}

void Fracture::Texture2DMultiSample::unbind() const
{
}

void Fracture::Texture2DMultiSample::GenerateMips()
{
}

uint32_t Fracture::Texture2DMultiSample::GetWidth() const
{
	return uint32_t();
}

uint32_t Fracture::Texture2DMultiSample::GetHeight() const
{
	return uint32_t();
}

uint32_t Fracture::Texture2DMultiSample::GetMipLevel() const
{
	return uint32_t();
}

void Fracture::Texture2DMultiSample::resize(int width, int height) const
{
}

uint32_t Fracture::Texture2DMultiSample::GetTextureID() const
{
	return uint32_t();
}

std::shared_ptr<Texture2DMultiSample> Fracture::Texture2DMultiSample::CreateTexture(InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
	return std::shared_ptr<Texture2DMultiSample>();
}

std::shared_ptr<Texture2DMultiSample> Fracture::Texture2DMultiSample::CreateTexture(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
	return std::shared_ptr<Texture2DMultiSample>();
}
