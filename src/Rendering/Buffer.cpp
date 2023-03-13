#include "FracturePCH.h"
#include "Buffer.h"

Fracture::VertexBufferLayout::VertexBufferLayout()
{
}

Fracture::VertexBufferLayout::VertexBufferLayout(const std::initializer_list<BufferAttribute>& attributes) :
	m_attributes(attributes)
{
	CalculateStride();
}

uint32_t Fracture::VertexBufferLayout::GetStride() const
{
	return m_Stride;
}

void Fracture::VertexBufferLayout::CalculateStride()
{
	uint32_t offset = 0;
	m_Stride = 0;
	for (const auto attribute : m_attributes)
	{
		attribute.UpdateOffset(offset);
		offset += attribute.Size;
		m_Stride += attribute.Size;
	}
}

Fracture::Buffer::Buffer() :
	RenderID(0)
{
}

Fracture::Buffer::~Buffer()
{	
}
