#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glm/glm.hpp>
#include <memory>
#include "BufferLayout.h"

namespace Fracture
{

	class VertexBuffer
	{

	public:
		VertexBuffer(unsigned int size);
		VertexBuffer(float* vertices, unsigned int size);
		~VertexBuffer();

		void bind();
		void unbind();

		const BufferLayout& GetLayout(){ return m_bufferlayout; }

		void SetLayout(const BufferLayout& layout){ m_bufferlayout = layout; }

		void setData(const void* data, unsigned int size);
		static std::shared_ptr<VertexBuffer> create(unsigned int size);
		static std::shared_ptr<VertexBuffer> create(float* vertices, unsigned int size);

	private:
		BufferLayout m_bufferlayout;
		unsigned int m_id;
	};

}

#endif
