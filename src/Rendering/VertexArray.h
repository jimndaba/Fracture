#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "Buffer.h"

namespace Fracture
{

	static uint32_t ShaderDataTypeSize(Fracture::ShaderDataType type)
	{
		switch (type)
		{
		case Fracture::ShaderDataType::Float:    return 4;
		case Fracture::ShaderDataType::Float2:   return 4 * 2;
		case Fracture::ShaderDataType::Float3:   return 4 * 3;
		case Fracture::ShaderDataType::Float4:   return 4 * 4;
		case Fracture::ShaderDataType::Mat3:     return 4 * 3 * 3;
		case Fracture::ShaderDataType::Mat4:     return 4 * 4 * 4;
		case Fracture::ShaderDataType::Int:      return 4;
		case Fracture::ShaderDataType::Int2:     return 4 * 2;
		case Fracture::ShaderDataType::Int3:     return 4 * 3;
		case Fracture::ShaderDataType::Int4:     return 4 * 4;
		case Fracture::ShaderDataType::Bool:     return 1;
		}
		return 0;
	};

	struct VertexArrayAttribute
	{
		VertexArrayAttribute(ShaderDataType shadertype, const std::string& name, int Divisor = 0) :
			Name(name), Type(shadertype), Offset(0), Size(ShaderDataTypeSize(shadertype)), divisor(Divisor)
		{
			if (Divisor > 0)
				Instanced = true;
		}

		~VertexArrayAttribute() = default;

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		mutable uint32_t Offset;
		bool Instanced = false;
		bool Interleaved = false;
		int divisor = 0;


		uint32_t GetCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3;
			case ShaderDataType::Mat4:    return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}

		void UpdateOffset(const uint32_t& offset) const
		{
			Offset = offset;
		}

	};

	struct VertexArrayLayout
	{
		VertexArrayLayout();
		VertexArrayLayout(const std::initializer_list<VertexArrayAttribute>& attributes)
			:Attributes(attributes)
		{
			calculateStride();
		}
		uint32_t GetStride() const;
		std::vector<VertexArrayAttribute> Attributes;

		void calculateStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (const auto& attribute : Attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}
		uint32_t m_Stride = 0;
	};

	struct VertexArrayCreationInfo
	{
		std::string Name;
		VertexArrayLayout Layout;
	};

	


}


#endif