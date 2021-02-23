#include "Texture2D.h"
#include "OpenGLBase.h"


Fracture::Texture2D::Texture2D(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType):Texture(),
    m_Width(width),
    m_Height(height),
    m_Format(format),
    m_TextureWrap(wrap),
    m_FormatType(formatType),
    m_InternalFormat(internalformat)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D,0, (GLenum)internalformat,width, height, 0,(GLenum)format, (GLenum)formatType, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

Fracture::Texture2D::Texture2D(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
    :Texture(),
    m_Width(width),
    m_Height(height),
    m_Format(format),
    m_TextureWrap(wrap),
    m_data(data),
    m_FormatType(formatType),
    m_InternalFormat(internalformat)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)internalformat, width, height, 0, (GLenum)format, (GLenum)formatType, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Fracture::Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Fracture::Texture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Fracture::Texture2D::GenerateMips()
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

uint32_t Fracture::Texture2D::TextureUnit() const
{
	return uint32_t();
}

uint32_t Fracture::Texture2D::GetWidth() const
{
    return m_Width;
}

uint32_t Fracture::Texture2D::GetHeight() const
{
    return m_Height;
}

uint32_t Fracture::Texture2D::GetMipLevel() const
{
    return m_MipLevels;
}

void Fracture::Texture2D::resize(int width, int height) const
{
    if (m_data)
    {
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)m_Format, width,height, 0, (GLenum)m_Format, GL_FLOAT,m_data);
    }   
}

uint32_t Fracture::Texture2D::GetTextureID() const
{
    return m_TextureID;
}

std::shared_ptr<Fracture::Texture2D> Fracture::Texture2D::CreateTexture(InternalFormat internalformat,TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
    return std::make_shared<Texture2D>(internalformat,format,width,height,wrap,formatType);
}

std::shared_ptr<Fracture::Texture2D> Fracture::Texture2D::CreateTexture(void* data, InternalFormat internalformat, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap, FormatType formatType)
{
    return std::make_shared<Texture2D>(data,internalformat,format, width, height, wrap,formatType);
}
