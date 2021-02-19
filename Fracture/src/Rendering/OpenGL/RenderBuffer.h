#pragma once
#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <memory>

namespace Fracture
{
	enum class InternalFormat;

	class RenderBuffer
	{

	public:
		RenderBuffer(InternalFormat format, uint32_t width, uint32_t height);
		RenderBuffer(uint32_t noSamples, InternalFormat format, uint32_t width, uint32_t height);
		~RenderBuffer();
		void bind();
		void unbind();

		void Resize(uint32_t width, uint32_t height);

		static std::shared_ptr<RenderBuffer> CreateBuffer(InternalFormat format, uint32_t width, uint32_t height);
		static std::shared_ptr<RenderBuffer> CreateBuffer(uint32_t noSamples,InternalFormat format, uint32_t width,uint32_t height);

		uint32_t GetBuffer();

		uint32_t NoSamples;
		uint32_t Width;
		uint32_t Height;

	private:
		uint32_t m_Id;
		InternalFormat m_format;
	};


}


#endif