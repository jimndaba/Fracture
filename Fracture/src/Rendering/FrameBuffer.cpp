#include "FrameBuffer.h"
#include "GLAD/glad.h"
#include <iostream>

Fracture::FrameBuffer::FrameBuffer()
{
	Invalidate();
}

Fracture::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_id);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteTextures(1, &m_DepthAttachment);
}

void Fracture::FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Fracture::FrameBuffer::unbind()
{	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fracture::FrameBuffer::Invalidate()
{
	if (m_id)
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	glCreateFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	std::cout<<(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!")<<std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fracture::FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		std::cout<<("Attempted to rezize framebuffer to {0}, {1}", width, height)<<std::endl;
		return;
	}
	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

unsigned int Fracture::FrameBuffer::GetColorAttachmentRendererID() const
{
	return m_ColorAttachment;
}

const Fracture::FrameBufferSpecification& Fracture::FrameBuffer::GetSpecification() const
{
	return m_Specification;
}

std::shared_ptr<Fracture::FrameBuffer> Fracture::FrameBuffer::Create(const FrameBufferSpecification& spec)
{
	return std::shared_ptr<FrameBuffer>(new FrameBuffer());
}
