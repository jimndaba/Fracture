#pragma once
#ifndef RENDERTARGET_H
#define REDNERTARGET_H

#include <vector>
#include <memory>
#include "GLAD/glad.h"

namespace Fracture
{

	class Texture;

	class RenderTarget
	{

	public:
		RenderTarget(unsigned int width, unsigned int height, GLenum type = GL_UNSIGNED_BYTE, unsigned int nrColorAttachments = 1, bool depthAndStencil = true);
		~RenderTarget();

		unsigned int ID;
		unsigned int Width;
		unsigned int Height;
		GLenum       Type;

		bool HasDepthAndStencil;

		std::shared_ptr<Texture> GetDepthStencilTexture();
		std::shared_ptr<Texture> GetColorTexture(unsigned int index);

		void bind();
		void Unbind();

		void Resize(unsigned int width, unsigned int height);
		void SetTarget(GLenum target);

	private:		
		GLenum									m_Target = GL_TEXTURE_2D;
		std::shared_ptr<Texture>				m_DepthStencil;
		std::vector<std::shared_ptr<Texture>>	m_ColorAttachments;
	};


}


#endif
