#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>

namespace Fracture
{
	struct FrameBufferSpecification
	{
		unsigned int Width = 0, Height = 0;
		// FramebufferFormat Format = 
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void bind();
		void unbind();
		void Invalidate();

		void Resize(unsigned int width, unsigned int height);

		unsigned int GetColorAttachmentRendererID() const;

		const FrameBufferSpecification& GetSpecification() const;

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

	private:
		unsigned int m_id = 0;
		unsigned int m_ColorAttachment = 0, m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;
		static const uint32_t s_MaxFramebufferSize = 8192;
	};

}

#endif