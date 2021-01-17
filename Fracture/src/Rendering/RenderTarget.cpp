#include "RenderTarget.h"
#include "Texture.h"
#include "Logging/Logger.h"
#include <iostream>
#include "Renderer.h"


Fracture::RenderTarget::RenderTarget(unsigned int width, unsigned int height, GLenum type, unsigned int nrColorAttachments, bool depthAndStencil)
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
                
        std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture("cAttachment" + i, width, height, internalFormat, GL_RGBA, GL_FLOAT, TextureType::ColorAttachment));

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,texture->id, 0);
        m_ColorAttachments.push_back(texture);
    }
    // then generate Depth/Stencil texture if requested
    HasDepthAndStencil = depthAndStencil;
    if (depthAndStencil)
    {
        

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,width,height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        std::shared_ptr<Texture> dtexture = std::shared_ptr<Texture>(new Texture("cDepthStencil", width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, TextureType::DepthStencilAttachment));
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,dtexture->id, 0);
        m_DepthStencil = dtexture;

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
    m_Target = target;
}

void Fracture::RenderTarget::BindAsBuffer(Renderer& renderer)
{
}

void Fracture::RenderTarget::BindAsBuffer(Renderer& renderer, BufferResource* resource)
{

}

void Fracture::RenderTarget::Clear(Renderer& renderer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    renderer.clearColor(1.0f,1.0f,1.0f);
    renderer.clear();



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fracture::RenderTarget::Clear(Renderer& renderer, glm::vec3 color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    renderer.clearColor(color.r,color.g,color.b);
    renderer.clear();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

