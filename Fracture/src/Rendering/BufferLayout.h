#pragma once
#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include "BufferElement.h"
#include <vector>

namespace Fracture
{
	//class BufferElement;

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(std::initializer_list<BufferElement>& elements);

		unsigned int GetStride() const { return m_Stride; }

		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride();
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;

	};

}

#endif
