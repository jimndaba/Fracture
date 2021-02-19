#include "RenderTarget.h"
#include "Texture.h"
#include "Logging/Logger.h"
#include <iostream>
#include "Renderer.h"
#include "OpenGL/OpenGLBase.h"
#include "OpenGL/FrameBuffer.h"
#include "OpenGL/RenderBuffer.h"

Fracture::RenderTarget::RenderTarget(const std::string& name):
    m_name(name),
    m_framebuffer(FrameBuffer::CreateBuffer())
{

}

Fracture::RenderTarget::RenderTarget(const std::string& name, unsigned int width, unsigned int height, TextureTarget texturetarget, GLenum type, unsigned int nrColorAttachments, bool depthAndStencil)
:m_name(name),
m_framebuffer(FrameBuffer::CreateBuffer()),
Width(width),
Height(height),
Type(type),
HasDepthAndStencil(depthAndStencil)
{      
    m_framebuffer->bind();
    for (unsigned int i = 0; i < nrColorAttachments; ++i)
    {
        GLenum internalFormat = GL_RGBA;
        if (type == GL_HALF_FLOAT)
            internalFormat = GL_RGBA16F;
        else if (type == GL_FLOAT)
            internalFormat = GL_RGBA32F;

        std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture("cAttachment" + i, width, height, texturetarget, internalFormat, GL_RGBA, GL_FLOAT, TextureType::ColorAttachment));

        if (texturetarget == TextureTarget::Texture2D)
        {
            m_framebuffer->AddAttachment(glAttachmentType::Color, i, glAttachmentTarget::Texture2D, texture->id);
        }
        if (texturetarget == TextureTarget::MultiSample)
        {
            m_framebuffer->AddAttachment(glAttachmentType::Color, i, glAttachmentTarget::MULTISAMPLE, texture->id);
        }
      
        m_ColorAttachments.push_back(texture);
    }

    if (depthAndStencil && texturetarget == TextureTarget::Texture2D)
    {           
        std::shared_ptr<Texture> dtexture = std::shared_ptr<Texture>(new Texture("cDepthStencil", width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, TextureType::DepthStencilAttachment));  
        m_framebuffer->AddAttachment(glAttachmentType::DepthStencil,glAttachmentTarget::Texture2D, dtexture->id);
        m_DepthStencil = dtexture;
    }

    if (depthAndStencil && texturetarget == TextureTarget::MultiSample)
    {
        m_framebuffer->AddRenderBuffer(name,glAttachmentType::DepthStencil, RenderBuffer::CreateBuffer(4, InternalFormat::Depth24Stencil8, width, height));
    }

    if (depthAndStencil && texturetarget == TextureTarget::CubeMap)
    {
        m_framebuffer->bind();
        std::shared_ptr<Texture> dtexture = std::shared_ptr<Texture>(new Texture("cDepthStencil", width, height,TextureTarget::CubeMap, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, TextureType::DepthStencilAttachment));
        m_DepthStencil = dtexture;
        m_framebuffer->AddAttachment(glAttachmentType::Depth, dtexture->id);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    m_framebuffer->unbind();
}

Fracture::RenderTarget::~RenderTarget()
{    

}

std::shared_ptr<Fracture::Texture> Fracture::RenderTarget::GetDepthStencilTexture()
{
    return m_DepthStencil;
}

std::shared_ptr<Fracture::Texture> Fracture::RenderTarget::GetColorTexture(unsigned int index)
{
    if (index < m_ColorAttachments.size())
        return m_ColorAttachments[index];
    else
    {
        std::cout << "RenderTarget color texture requested, but not available: " + std::to_string(index) << std::endl;
        return nullptr;
    }
}

void Fracture::RenderTarget::bind()
{
    m_framebuffer->bind();
}

void Fracture::RenderTarget::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight)
{
    m_framebuffer->blit(otherBuffer,SrcDstWidth,SrcDstheight);
}

void Fracture::RenderTarget::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight)
{
    m_framebuffer->blit(otherBuffer, srcWidth,srcHeight,dstWidth,dstHeight);
}

void Fracture::RenderTarget::Unbind()
{   
    m_framebuffer->unbind();
}

void Fracture::RenderTarget::Resize(unsigned int width, unsigned int height)
{
    if (m_IsResizable)
    {
        Width = width;
        Height = height;

        for (unsigned int i = 0; i < m_ColorAttachments.size(); ++i)
        {
            m_ColorAttachments[i]->Resize(width, height);
        }
        // generate Depth/Stencil texture if requested
        if (HasDepthAndStencil)
        {
            m_DepthStencil->Resize(width, height);
        }
    }
}

void Fracture::RenderTarget::SetTarget(GLenum target)
{
 
}

void Fracture::RenderTarget::SetMultiSampled(bool value)
{
    m_IsMultiSampled = value;
}

bool Fracture::RenderTarget::IsMultiSampled()
{
    return  m_IsMultiSampled;
}

void Fracture::RenderTarget::SetResizable(bool value)
{
    m_IsResizable = value;
}

std::shared_ptr<Fracture::RenderTarget> Fracture::RenderTarget::CreateRenderTarget(const std::string& name)
{
    return std::make_shared<RenderTarget>(name);
}

std::shared_ptr<Fracture::RenderTarget> Fracture::RenderTarget::CreateRenderTarget(const std::string& name, unsigned int width, unsigned int height, TextureTarget texturetarget, GLenum type, unsigned int nrColorAttachments, bool depthAndStencil)
{
    return std::make_shared<RenderTarget>(name,width,height,texturetarget,type,nrColorAttachments,depthAndStencil);
}

bool Fracture::RenderTarget::IsResizable()
{
    return m_IsResizable;
}

std::shared_ptr<Fracture::FrameBuffer> Fracture::RenderTarget::GetBuffer()
{
    return m_framebuffer;
}

