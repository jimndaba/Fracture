#include "Texture2DMultiSample.h"
#include "OpenGLBase.h"

Fracture::Texture2DMultiSample::Texture2DMultiSample(InternalFormat internalformat, uint32_t noSamples, uint32_t width, uint32_t height):Texture(),
m_Width(width),
m_Height(height),
m_InternalFormat(internalformat),
m_NoSamples(noSamples)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, noSamples, (GLenum)internalformat, width, height, GL_TRUE);
}


void Fracture::Texture2DMultiSample::bind() const
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
}

void Fracture::Texture2DMultiSample::unbind() const
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void Fracture::Texture2DMultiSample::GenerateMips()
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
	glGenerateMipmap(GL_TEXTURE_2D_MULTISAMPLE);
}

uint32_t Fracture::Texture2DMultiSample::GetWidth() const
{
	return m_Width;
}

uint32_t Fracture::Texture2DMultiSample::GetHeight() const
{
	return m_Height;
}

uint32_t Fracture::Texture2DMultiSample::GetMipLevel() const
{
	return m_MipLevels;
}

void Fracture::Texture2DMultiSample::resize(int width, int height) const
{
	bind();
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,m_NoSamples,(GLenum)m_InternalFormat, width, height, GL_TRUE);
}

uint32_t Fracture::Texture2DMultiSample::GetTextureID() const
{
	return m_TextureID;
}

std::shared_ptr<Fracture::Texture2DMultiSample> Fracture::Texture2DMultiSample::CreateTexture(InternalFormat internalformat, uint32_t noSamples, uint32_t width, uint32_t height)
{
	return std::make_shared<Texture2DMultiSample>(internalformat,noSamples,width,height);
}
