#include "TextureCubeMap.h"
#include "OpenGLBase.h"

Fracture::TextureCubeMap::TextureCubeMap(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap,FormatType formatType) :Texture(),
m_Width(width),
m_Height(height),
m_Format(format),
m_TextureWrap(wrap),
m_FormatType(formatType),
m_InternalFormat(internalformat)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Fracture::TextureCubeMap::TextureCubeMap(void* data,InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType):Texture(),
m_Width(width),
m_Height(height),
m_Format(format),
m_TextureWrap(wrap),
m_data(data),
m_FormatType(formatType),
m_InternalFormat(internalformat)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,(GLenum)internalformat,
			width, height, 0, (GLenum)format, (GLenum)formatType, data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Fracture::TextureCubeMap::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void Fracture::TextureCubeMap::unbind() const
{	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Fracture::TextureCubeMap::GenerateMips()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

uint32_t Fracture::TextureCubeMap::GetWidth() const
{
	return m_Width;
}

uint32_t Fracture::TextureCubeMap::GetHeight() const
{
	return m_Height;
}

uint32_t Fracture::TextureCubeMap::GetMipLevel() const
{
	return uint32_t();
}

void Fracture::TextureCubeMap::resize(int width, int height) const
{
}

uint32_t Fracture::TextureCubeMap::GetTextureID() const
{
	return m_TextureID;
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::TextureCubeMap::CreateTexture(InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
	return std::make_shared<TextureCubeMap>(internalformat,format,width,height,wrap,formatType);
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::TextureCubeMap::CreateTexture(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
	return std::make_shared<TextureCubeMap>(data,internalformat, format, width, height, wrap, formatType);
}
