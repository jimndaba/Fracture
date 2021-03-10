#include "RenderTarget.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/Texture2DMultiSample.h"
#include "OpenGL/TextureCubeMap.h"
#include "Logging/Logger.h"
#include "Renderer.h"
#include "OpenGL/OpenGLBase.h"
#include "OpenGL/FrameBuffer.h"
#include "OpenGL/RenderBuffer.h"

Fracture::RenderTarget::RenderTarget(const std::string& name):
    m_name(name),
    m_framebuffer(FrameBuffer::CreateBuffer())
{
}

Fracture::RenderTarget::RenderTarget(const std::string& name, unsigned int width, unsigned int height, AttachmentTarget texturetarget, FormatType type, unsigned int nrColorAttachments, bool depthAndStencil)
:m_name(name),
m_framebuffer(FrameBuffer::CreateBuffer()),
Width(width),
Height(height),
m_Type(type),
HasDepthAndStencil(depthAndStencil)
{   
    m_framebuffer->bind();    

    for (unsigned int i = 0; i < nrColorAttachments; ++i)
    {
  
        InternalFormat internalFormat = InternalFormat::RGBA;
        if (type == FormatType::Half_Float)
            internalFormat = InternalFormat::RedGreen16;
        else if (type == FormatType::Float)
            internalFormat = InternalFormat::RGBA32;
        else if (type == FormatType::UInt)
            internalFormat = InternalFormat::RGB;

        if (texturetarget == AttachmentTarget::Texture2D)
        {
            std::shared_ptr<Texture2D> texture = Texture2D::CreateTexture(internalFormat,TextureFormat::RGB,width,height,glWrap::ClampToEdge,type);

            m_framebuffer->AddAttachment(AttachmentType::Color, i, AttachmentTarget::Texture2D, texture->GetTextureID());

            m_ColorAttachments.push_back(texture);
        }
        if (texturetarget == AttachmentTarget::MULTISAMPLE)
        {
            std::shared_ptr<Texture2DMultiSample> texture = Texture2DMultiSample::CreateTexture(internalFormat,4, width, height);
            m_framebuffer->AddAttachment(AttachmentType::Color, i, AttachmentTarget::MULTISAMPLE, texture->GetTextureID());
            m_ColorAttachments.push_back(texture);
        }      
       
    }

    if (m_ColorAttachments.size() > 1)
    {
        int size = m_ColorAttachments.size();
        unsigned int attachments[10];

        for (int i = 0; i < m_ColorAttachments.size(); i++)
        {           
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(size, attachments);
    }

    if (depthAndStencil && texturetarget == AttachmentTarget::Texture2D)
    {       
        std::shared_ptr<Texture2D> texture = Texture2D::CreateTexture(InternalFormat::Depth24Stencil8, TextureFormat::DepthStencil, width, height, glWrap::ClampToEdge, FormatType::UInt24_8);

        m_framebuffer->AddAttachment(AttachmentType::DepthStencil,AttachmentTarget::Texture2D, texture->GetTextureID());
        m_DepthStencil = texture;
    }

    if (depthAndStencil && texturetarget == AttachmentTarget::MULTISAMPLE)
    {
        m_framebuffer->AddRenderBuffer(name,AttachmentType::DepthStencil, RenderBuffer::CreateBuffer(4, InternalFormat::Depth24Stencil8, width, height));
    }

    if (depthAndStencil && texturetarget == AttachmentTarget::CubeMapPosX)
    {
        std::shared_ptr<TextureCubeMap> texture = TextureCubeMap::CreateTexture(InternalFormat::Depth24Stencil8, TextureFormat::DepthStencil, width, height, glWrap::ClampToEdge, FormatType::UInt24_8,false);

        m_framebuffer->AddAttachment(AttachmentType::Depth, texture->GetTextureID());
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        m_DepthStencil = texture;
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

void Fracture::RenderTarget::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight, int attachment = 0)
{
    m_framebuffer->blit(otherBuffer,SrcDstWidth,SrcDstheight,attachment);
}

void Fracture::RenderTarget::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight, int attachment = 0)
{
    m_framebuffer->blit(otherBuffer, srcWidth,srcHeight,dstWidth,dstHeight,attachment);
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


std::shared_ptr<Fracture::RenderTarget> Fracture::RenderTarget::CreateRenderTarget(const std::string& name, unsigned int width, unsigned int height, AttachmentTarget texturetarget, FormatType type, unsigned int nrColorAttachments, bool depthAndStencil)
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

