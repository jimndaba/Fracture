#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>
#include <memory>

namespace Fracture
{
	class VertexBuffer;
	class IndexBuffer;

	class VertexArray
	{

	public:
		VertexArray();
		~VertexArray();

		void bind();
		void unbind();

		unsigned int m_Id;

		static std::shared_ptr<VertexArray> create();

		void addVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
		void setIndexBuffer(std::shared_ptr<IndexBuffer> index);

		std::vector<std::shared_ptr<VertexBuffer>>& getBuffers();
		std::shared_ptr<IndexBuffer>& getIndex();

	private:

		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};

}

#endif
