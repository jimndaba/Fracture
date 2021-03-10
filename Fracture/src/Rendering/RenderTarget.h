#pragma once
#ifndef RENDERTARGET_H
#define REDNERTARGET_H

#include <vector>
#include <memory>
#include <string>
#include "glm/glm.hpp"
#include <iostream>
#include "FrameGraph/FrameResource.h"
#include "Bindable.h"

namespace Fracture
{

	class Texture;
	class Texture2D;
	enum class AttachmentTarget;
	class FrameBuffer;
	enum class FormatType;


	class RenderTarget : public Bindable,public FrameResource
	{

	public:
		RenderTarget(const std::string& name);
		RenderTarget(const std::string& name,unsigned int width, unsigned int height, AttachmentTarget texturetarget,FormatType type, unsigned int nrColorAttachments = 1, bool depthAndStencil = false);
		~RenderTarget();
		
		unsigned int Width;
		unsigned int Height;
		FormatType m_Type;
	
		bool HasRenderBuffer;
		bool HasDepthAndStencil;

		std::shared_ptr<Texture> GetDepthStencilTexture() override;
		std::shared_ptr<Texture> GetColorTexture(unsigned int index) override;

		std::shared_ptr<FrameBuffer> GetBuffer() override;
		
		void bind() override;
		void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight, int attachment) override;
		void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight,int attachment) override;
		void Unbind();

		void Resize(unsigned int width, unsigned int height);
		
		bool IsMultiSampled() override;
		bool IsResizable() override;

		void SetMultiSampled(bool value) override;
		void SetResizable(bool value)override;
		
		static std::shared_ptr<RenderTarget> CreateRenderTarget(const std::string& name);
		static std::shared_ptr<RenderTarget> CreateRenderTarget(const std::string& name, unsigned int width, unsigned int height, AttachmentTarget texturetarget, FormatType type, unsigned int nrColorAttachments = 1, bool depthAndStencil = false);

	private:		
		std::shared_ptr<FrameBuffer> m_framebuffer;
		std::string m_name;
		bool m_IsMultiSampled;
		bool m_IsResizable = true;
		//GLenum									m_Target = GL_TEXTURE_2D;
		std::shared_ptr<Texture>				m_DepthStencil;
		std::vector<std::shared_ptr<Texture>>	m_ColorAttachments;
	};


}


#endif
