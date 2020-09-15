#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "BufferLayout.h"
#include <glm/glm.hpp>
#include <memory>


namespace Fracture
{
	class Vertex;


	class VertexBuffer
	{

	public:
		VertexBuffer(unsigned int size);
		VertexBuffer(float* vertices, unsigned int size);
		VertexBuffer(std::vector<Vertex> vertices, unsigned int size);
		~VertexBuffer();

		void bind();
		void unbind();

		const BufferLayout& GetLayout(){ return *m_bufferlayout; }

		void SetLayout(BufferLayout* layout){ m_bufferlayout = layout; }

		void setData(const void* data, unsigned int size);
		static std::shared_ptr<VertexBuffer> create(unsigned int size);
		static std::shared_ptr<VertexBuffer> create(std::vector<Vertex> vertices, unsigned int size);

	private:
		BufferLayout* m_bufferlayout;
		unsigned int m_id;
	};

}

#endif
