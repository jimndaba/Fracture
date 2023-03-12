#include "FracturePCH.h"
#include "VertexArray.h"

Fracture::VertexArrayLayout::VertexArrayLayout()
{
}

uint32_t Fracture::VertexArrayLayout::GetStride() const
{
	return m_Stride;
}
