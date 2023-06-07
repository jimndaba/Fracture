#pragma once
#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "Texture.h"

namespace Fracture
{


	struct RenderTargetCreationInfo
	{
		RenderTargetCreationInfo();
		RenderTargetCreationInfo(std::initializer_list< TextureCreationInfo> c_attachments);
		std::string Name;
		AttachmentTarget Target;
		std::vector<TextureCreationInfo> ColorAttachments;
		std::vector<TextureCreationInfo> DepthStencilAttachments;
		bool HasRenderBuffer = false;
		bool IsResizable = false;
		float Width = 1.0f;
		float Height = 1.0f;
	};

	struct RenderTarget : public IGraphicsResource
	{
		RenderTarget();

		RenderTargetCreationInfo Info;
		uint32_t Handle;
		std::vector<std::shared_ptr<Texture>> ColorAttachments;
		std::shared_ptr<Texture> DepthStencilAttachment;
		uint32_t RenderBufferHandle;
	private:
		// pure virtual implementation
		void bind() {};
	};
}

#endif