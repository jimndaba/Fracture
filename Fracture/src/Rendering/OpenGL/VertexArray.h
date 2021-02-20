#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <cstdint>
#include <memory>

namespace Fracture
{

	class VertexBuffer;


	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind();
		void unbind();

		void AddBuffer(const std::shared_ptr<VertexBuffer>& buffer);

		uint32_t RenderID();

		static std::shared_ptr<VertexArray> Create();

	private:
		uint32_t m_RenderID;
	};


}

#endif