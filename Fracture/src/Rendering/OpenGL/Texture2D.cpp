#include "Texture2D.h"
#include "OpenGLBase.h"


Fracture::Texture2D::Texture2D(TextureFormat format, uint32_t width, uint32_t height, glWrap wrap):
    m_Width(width),
    m_Height(height),
    m_Format(format),
    m_TextureWrap(wrap)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D,0, (GLenum)format,width, height, 0,(GLenum)format, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

Fracture::Texture2D::Texture2D(void* data, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap)
    :
    m_Width(width),
    m_Height(height),
    m_Format(format),
    m_TextureWrap(wrap)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)m_Format, width,height, 0, (GLenum)m_Format, GL_FLOAT, data);
}

uint32_t Fracture::Texture2D::GetTextureID() const
{
    return m_TextureID;
}

std::shared_ptr<Fracture::Texture2D> Fracture::Texture2D::CreateTexture(TextureFormat format, uint32_t width, uint32_t height, glWrap wrap)
{
    return std::make_shared<Texture2D>(format,width,height,wrap);
}

std::shared_ptr<Fracture::Texture2D> Fracture::Texture2D::CreateTexture(void* data, TextureFormat format, uint32_t width, uint32_t height, glWrap wrap)
{
    return std::make_shared<Texture2D>(data,format, width, height, wrap);
}
