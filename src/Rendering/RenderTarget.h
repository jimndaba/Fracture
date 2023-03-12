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
	};

	struct RenderTarget : public IGraphicsResource
	{
		RenderTarget();

		RenderTargetCreationInfo Info;
		uint32_t Handle;
		std::vector<std::shared_ptr<Texture>> ColorAttachments;
		std::shared_ptr<Texture> DepthStencilAttachment;
	private:
		// pure virtual implementation
		void bind() {};
	};
}

#endif