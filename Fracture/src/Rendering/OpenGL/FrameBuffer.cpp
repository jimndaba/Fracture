#include "FrameBuffer.h"
#include "OpenGLBase.h"
#include "RenderBuffer.h"
#include "Logging/Logger.h"

Fracture::FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &m_Id);	
}

void Fracture::FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
}

void Fracture::FrameBuffer::unbind()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		FRACTURE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer - {} - is not complete!");
		throw "FRAMEBUFFER_INVALID";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fracture::FrameBuffer::AddAttachment(AttachmentType type, uint32_t textureID)
{
	glFramebufferTexture(GL_FRAMEBUFFER,(GLenum)type, textureID, 0);
}

void Fracture::FrameBuffer::AddAttachment(AttachmentType type, AttachmentTarget attachmentTarget, uint32_t textureID)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)type, (GLenum)attachmentTarget, textureID, 0);
}

void Fracture::FrameBuffer::AddAttachment(AttachmentType type,uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t textureID)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER,(GLenum)type + attachmentIndex,(GLenum)attachmentTarget, textureID, 0);
}

void Fracture::FrameBuffer::AddAttachment(AttachmentType type, uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t textureID, uint32_t miplevels)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)type + attachmentIndex, (GLenum)attachmentTarget, textureID, miplevels);
}

void Fracture::FrameBuffer::AddAttachment(AttachmentType type, uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t targetindex, uint32_t textureID, uint32_t miplevels)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)type + attachmentIndex, (GLenum)attachmentTarget + targetindex, textureID, miplevels);
}

void Fracture::FrameBuffer::AddRenderBuffer(const std::string& name,AttachmentType attachmentType, const std::shared_ptr<RenderBuffer>& buffer)
{
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)attachmentType, GL_RENDERBUFFER, buffer->GetBuffer());
	m_RenderBuffers[name] = buffer;
}

std::shared_ptr<Fracture::RenderBuffer> Fracture::FrameBuffer::GetRenderBuffer(const std::string& name)
{
	auto& buffer = m_RenderBuffers[name];
	return buffer;
}

void Fracture::FrameBuffer::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, otherBuffer->GetBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Id);
	glBlitFramebuffer(0, 0, SrcDstWidth, SrcDstheight, 0, 0, SrcDstWidth, SrcDstheight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Fracture::FrameBuffer::blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, otherBuffer->GetBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Id);
	glBlitFramebuffer(0, 0,srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

std::shared_ptr<Fracture::FrameBuffer> Fracture::FrameBuffer::CreateBuffer()
{
	return std::make_shared<FrameBuffer>();
}

uint32_t Fracture::FrameBuffer::GetBufferID()
{
	return m_Id;
}
