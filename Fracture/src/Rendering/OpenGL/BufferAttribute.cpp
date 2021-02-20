#include "BufferAttribute.h"

Fracture::BufferAttribute::BufferAttribute(ShaderDataType Type, const std::string& Name):
	m_Name(Name),m_Type(Type),m_Size(ShaderDataTypeSize(Type)),m_Offset(0)
{
}

uint32_t Fracture::BufferAttribute::GetCount()const
{
	switch (m_Type)
	{
	case ShaderDataType::Float:   return 1;
	case ShaderDataType::Float2:  return 2;
	case ShaderDataType::Float3:  return 3;
	case ShaderDataType::Float4:  return 4;
	case ShaderDataType::Mat3:    return 3 * 3;
	case ShaderDataType::Mat4:    return 4 * 4;
	case ShaderDataType::Int:     return 1;
	case ShaderDataType::Int2:    return 2;
	case ShaderDataType::Int3:    return 3;
	case ShaderDataType::Int4:    return 4;
	case ShaderDataType::Bool:    return 1;
	}

	return 0;
}

std::string Fracture::BufferAttribute::Name()const
{
	return m_Name;
}

uint32_t Fracture::BufferAttribute::Size()const
{
	return m_Size;
}

uint32_t Fracture::BufferAttribute::Offset()const
{
	return m_Offset;
}

Fracture::ShaderDataType Fracture::BufferAttribute::Type() const
{
	return m_Type;
}

void Fracture::BufferAttribute::Offset(const uint32_t& offset)
{
	m_Offset = offset;
}
