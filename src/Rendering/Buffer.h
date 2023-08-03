#pragma once
#ifndef BUFFER_H
#define BUFFER_H

namespace Fracture
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
		Float4Instanced
	};

	enum class BufferUsage
	{
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream = GL_STREAM_DRAW,
		StaticCopy = GL_STATIC_COPY
	};

	enum class BufferType
	{
		ArrayBuffer = GL_ARRAY_BUFFER,
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
		UniformBuffer = GL_UNIFORM_BUFFER,
		TextureBuffer = GL_TEXTURE_BUFFER,
		FrameBuffer = GL_FRAMEBUFFER,
		ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
		DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER
	};

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
		uint32_t size;
		uint32_t count;
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