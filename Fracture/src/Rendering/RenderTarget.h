#pragma once
#ifndef RENDERTARGET_H
#define REDNERTARGET_H

#include <vector>
#include <memory>
#include "GLAD/glad.h"
#include "glm/glm.hpp"
#include "FrameGraph/FrameResource.h"
#include "Bindable.h"

namespace Fracture
{

	class Texture;
	enum class TextureTarget;

	class RenderTarget : public Bindable,public FrameResource
	{

	public:
		RenderTarget(unsigned int width, unsigned int height, TextureTarget texturetarget,GLenum type = GL_UNSIGNED_BYTE, unsigned int nrColorAttachments = 1, bool depthAndStencil = false);
		~RenderTarget();

		unsigned int ID;
		unsigned int Width;
		unsigned int Height;
		GLenum       Type;
	
		bool HasRenderBuffer;
		bool HasDepthAndStencil;

		std::shared_ptr<Texture> GetDepthStencilTexture() override;
		std::shared_ptr<Texture> GetColorTexture(unsigned int index) override;

		unsigned int GetID() override { return ID; };
		void bind() override;
		void blit(unsigned int fbo) override;
		void Unbind();

		void Resize(unsigned int width, unsigned int height);
		void SetTarget(GLenum target);

		void SetMultiSampled(bool value) override;
		bool IsMultiSampled() override;

	private:		
		bool m_IsMultiSampled;
		//GLenum									m_Target = GL_TEXTURE_2D;
		std::shared_ptr<Texture>				m_DepthStencil;
		std::vector<std::shared_ptr<Texture>>	m_ColorAttachments;
	};


}


#endif
