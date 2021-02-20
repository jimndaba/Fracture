#include "VertexBufferLayout.h"
#include "BufferAttribute.h"

Fracture::VertexBufferLayout::VertexBufferLayout()
{
}

Fracture::VertexBufferLayout::VertexBufferLayout(const std::initializer_list<BufferAttribute>& attributes):
	m_attributes(attributes)
{
	CalculateStride();
}

std::vector<Fracture::BufferAttribute> Fracture::VertexBufferLayout::GetAttributes() const
{
	return m_attributes;
}

uint32_t Fracture::VertexBufferLayout::GetStride()const
{
	return m_Stride;
}

void Fracture::VertexBufferLayout::CalculateStride()
{
	uint32_t offset = 0;
	m_Stride = 0;
	for (auto attribute : m_attributes)
	{
		attribute.Offset(offset);
		offset += attribute.Size();
		m_Stride += attribute.Size();
	}
}