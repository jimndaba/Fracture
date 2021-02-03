#include "RenderTarget.h"
#include "Texture.h"
#include "Logging/Logger.h"
#include <iostream>
#include "Renderer.h"

Fracture::RenderTarget::RenderTarget(unsigned int width, unsigned int height, TextureTarget texturetarget, GLenum type, unsigned int nrColorAttachments, bool depthAndStencil)
{
    Width = width;
    Height = height;
    Type = type;

    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    // generate all requested color attachments
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
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->id, 0);
        }
        if (texturetarget == TextureTarget::MultiSample)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, texture->id, 0);
        }
      
        m_ColorAttachments.push_back(texture);
    }
    // then generate Depth/Stencil texture if requested
    HasDepthAndStencil = depthAndStencil;
    if (depthAndStencil && texturetarget == TextureTarget::Texture2D)
    {       
        
        std::shared_ptr<Texture> dtexture = std::shared_ptr<Texture>(new Texture("cDepthStencil", width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, TextureType::DepthStencilAttachment));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, dtexture->id, 0);
        m_DepthStencil = dtexture;
    }

    if (depthAndStencil && texturetarget == TextureTarget::MultiSample)
    {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    if (depthAndStencil && texturetarget == TextureTarget::CubeMap)
    {

        std::shared_ptr<Texture> dtexture = std::shared_ptr<Texture>(new Texture("cDepthStencil", width, height,TextureTarget::CubeMap, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, TextureType::DepthStencilAttachment));
        m_DepthStencil = dtexture;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dtexture->id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        FRACTURE_ERROR("Framebuffer not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Fracture::RenderTarget::~RenderTarget()
{

    glDeleteFramebuffers(1,&ID);

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
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Fracture::RenderTarget::blit(unsigned int fbo)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
    glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

void Fracture::RenderTarget::Unbind()
{   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fracture::RenderTarget::Resize(unsigned int width, unsigned int height)
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

