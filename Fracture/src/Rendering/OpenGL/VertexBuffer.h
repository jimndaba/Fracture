#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <memory>
#include "VertexBufferLayout.h"

namespace Fracture
{

	class VertexBuffer
	{
	public:
		VertexBuffer(void* data, uint32_t size);
		~VertexBuffer();

		void SetData(void* buffer, uint32_t offset, uint32_t size);
		void bind() const;
	   
		const VertexBufferLayout& GetLayout() const;
		void SetLayout(const VertexBufferLayout& layout);
		
		uint32_t GetRenderID();

		static std::shared_ptr<VertexBuffer> Create(void* data, uint32_t size);

	private:
		uint32_t m_RenderID;
		VertexBufferLayout m_layout;
	};


}

#endif