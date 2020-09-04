#include "BufferLayout.h"
#include "BufferElement.h"

Fracture::BufferLayout::BufferLayout()
{
}

Fracture::BufferLayout::BufferLayout(std::initializer_list<BufferElement>& elements):
	m_Elements(elements)
{
	CalculateOffsetsAndStride();
}

void Fracture::BufferLayout::CalculateOffsetsAndStride()
{	
	size_t offset = 0;
	m_Stride = 0;
	for (auto& element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}
	
}
