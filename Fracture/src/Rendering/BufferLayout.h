#pragma once
#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H


#include <vector>
#include <initializer_list>
#include "BufferElement.h"

namespace Fracture
{

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(std::initializer_list<BufferElement> elements);

		unsigned int GetStride(){ return m_Stride; }

		std::vector<BufferElement> GetElements(){ return m_Elements; }
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }	

		void addElement(BufferElement element);

	private:
		void CalculateOffsetsAndStride();
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;

	};

}

#endif
