#pragma once
#ifndef RENDERTARGET_H
#define REDNERTARGET_H

#include <vector>
#include <memory>
#include "GLAD/glad.h"
#include "glm/glm.hpp"
#include "FrameGraph/FrameResource.h"
#include "RenderGraph/BufferResource.h"
#include "Bindable.h"

namespace Fracture
{

	class Texture;

	class RenderTarget : public BufferResource, public Bindable,public FrameResource
	{

	public:
		RenderTarget(unsigned int width, unsigned int height, GLenum type = GL_UNSIGNED_BYTE, unsigned int nrColorAttachments = 1, bool depthAndStencil = true);
		~RenderTarget();

		unsigned int ID;
		unsigned int Width;
		unsigned int Height;
		GLenum       Type;

		bool HasDepthAndStencil;

		std::shared_ptr<Texture> GetDepthStencilTexture() override;
		std::shared_ptr<Texture> GetColorTexture(unsigned int index) override;

		void bind() override;
		void Unbind();

		void Resize(unsigned int width, unsigned int height);
		void SetTarget(GLenum target);

		//RenderGraph Resource functions
		virtual void BindAsBuffer(Renderer& renderer);
		virtual void BindAsBuffer(Renderer& renderer, BufferResource* resource);
		virtual void Clear(Renderer& renderer);
		virtual void Clear(Renderer& renderer,glm::vec3 color);

	private:		
		GLenum									m_Target = GL_TEXTURE_2D;
		std::shared_ptr<Texture>				m_DepthStencil;
		std::vector<std::shared_ptr<Texture>>	m_ColorAttachments;
	};


}


#endif
