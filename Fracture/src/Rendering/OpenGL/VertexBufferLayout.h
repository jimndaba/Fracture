#pragma once
#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include "BufferAttribute.h"

namespace Fracture
{

	class VertexBufferLayout
	{

	public:
		VertexBufferLayout();
		VertexBufferLayout(const std::initializer_list<BufferAttribute>& attributes);
		~VertexBufferLayout() = default;

		std::vector<BufferAttribute> GetAttributes() const;

		uint32_t GetStride() const;

	private:
		void CalculateStride();


	private:
		std::vector<BufferAttribute> m_attributes;
		uint32_t m_Stride = 0;
	};


}

#endif