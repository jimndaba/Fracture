#pragma once
#ifndef FRAMERESOURCE_H
#define FRAMERESOURCE_H

namespace Fracture
{
	class Texture;
	class FrameBuffer;

	class FrameResource
	{
	public:
		FrameResource() {};
		virtual void bind() = 0;
		virtual void Unbind() {};
		virtual std::shared_ptr<FrameBuffer> GetBuffer() = 0;
		virtual void Resize(unsigned int width, unsigned int height) {};
		
		virtual void SetMultiSampled(bool value) { };
		virtual void SetResizable(bool value) {};
		virtual bool IsMultiSampled() { return false; };
		virtual bool IsResizable() { return true; };
		virtual void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& SrcDstWidth, const uint32_t& SrcDstheight ,int attachment = 0){ };
		virtual void blit(const std::shared_ptr<FrameBuffer>& otherBuffer, const uint32_t& srcWidth, const uint32_t& srcHeight, const uint32_t& dstWidth, const uint32_t& dstHeight, int attachment = 0) { };
		virtual std::shared_ptr<Texture> GetDepthStencilTexture() { return nullptr; };
		virtual std::shared_ptr<Texture> GetColorTexture(unsigned int index) { return nullptr; };
	private:
	};

}

#endif