#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include "RenderCommands.h"

namespace Fracture
{
	
	struct BufferAttribute
	{
		BufferAttribute(ShaderDataType shadertype, const std::string& name) :
			Name(Name), Type(shadertype), Offset(0)
		{

		}
		~BufferAttribute() = default;

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		mutable uint32_t Offset;

		uint32_t GetCount() const
		{
			switch (Type)
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
			case ShaderDataType::Float4Instanced:    return 4;
			}

			return 0;
		}

		void UpdateOffset(const uint32_t& offset) const
		{
			Offset += offset;
		}

	};

	struct VertexBufferLayout
	{
		VertexBufferLayout();
		VertexBufferLayout(const std::initializer_list<BufferAttribute>& attributes);

		uint32_t GetStride() const;

	private:
		void CalculateStride();

		std::vector<BufferAttribute> m_attributes;
		uint32_t m_Stride = 0;
	};

	struct BufferDescription
	{
		std::string Name;
		BufferType bufferType;
		BufferUsage usage;
		uint32_t BufferAccessFlags;
		uint32_t size;
		uint32_t count;
		bool IsPersistantlyMapped = false;		
		const void* data;
		VertexBufferLayout m_layout;
	};

	struct Buffer
	{
	public:
		Buffer();
		~Buffer();
		uint32_t RenderID;
		BufferDescription Description;
	};





}

#endif