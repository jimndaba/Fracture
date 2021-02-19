#include "RenderBuffer.h"
#include "OpenGLBase.h"

Fracture::RenderBuffer::RenderBuffer(InternalFormat format, uint32_t width, uint32_t height):
    m_format(format),
    Width(width),
    Height(height)
{
    glGenRenderbuffers(1, &m_Id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
    glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)format, width, height);
}

Fracture::RenderBuffer::RenderBuffer(uint32_t noSamples, InternalFormat format, uint32_t width, uint32_t height):
    NoSamples(noSamples),
    m_format(format),
    Width(width),
    Height(height)
{
    glGenRenderbuffers(1, &m_Id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,noSamples,(GLenum)format, width, height);
}

Fracture::RenderBuffer::~RenderBuffer()
{
    glDeleteBuffers(1, &m_Id);
}

void Fracture::RenderBuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id); 
}

void Fracture::RenderBuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Fracture::RenderBuffer::Resize(uint32_t width, uint32_t height)
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
    glRenderbufferStorage(GL_RENDERBUFFER,(GLenum)m_format,width,height);
}

std::shared_ptr<Fracture::RenderBuffer> Fracture::RenderBuffer::CreateBuffer(InternalFormat format, uint32_t width, uint32_t height)
{
    return std::make_shared<RenderBuffer>(format, width, height);
}

std::shared_ptr<Fracture::RenderBuffer> Fracture::RenderBuffer::CreateBuffer(uint32_t noSamples, InternalFormat format, uint32_t width, uint32_t height)
{
    return std::make_shared<RenderBuffer>(noSamples,format,width,height);
}

uint32_t Fracture::RenderBuffer::GetBuffer()
{
    return m_Id;
}
