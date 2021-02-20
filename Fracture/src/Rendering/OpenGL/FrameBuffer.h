#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <map>
#include <string>

namespace Fracture
{
	enum class AttachmentTarget;
	enum class AttachmentType;
	class RenderBuffer;

	class FrameBuffer
	{
	public:
		FrameBuffer();

		void bind();
		void unbind();
		void AddAttachment(AttachmentType type, uint32_t textureID);
		void AddAttachment(AttachmentType type, AttachmentTarget attachmentTarget, uint32_t textureID);	
		void AddAttachment(AttachmentType type, uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t textureID);
		void AddAttachment(AttachmentType type, uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t textureID, uint32_t miplevels);
		void AddAttachment(AttachmentType type, uint32_t attachmentIndex, AttachmentTarget attachmentTarget, uint32_t targetindex, uint32_t textureID, uint32_t miplevels);
		
		void AddRenderBuffer(const std::string& name,AttachmentType attachmentType,const std::shared_ptr<RenderBuffer>& buffer);
		std::shared_ptr<RenderBuffer> GetRenderBuffer(const std::string& name);

		void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight);
		void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight);

		/// <summary>
		/// TO DO  - Got to add blit functionallity here.
		/// </summary>
		/// <returns></returns>

		static std::shared_ptr<FrameBuffer> CreateBuffer();
		uint32_t GetBufferID();
	private:
		uint32_t m_Id;

		std::map<std::string, std::shared_ptr<RenderBuffer>> m_RenderBuffers;
	
	};
}

#endif