#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BufferElement.h"
#include "IndexBuffer.h"
#include <GLAD/glad.h>
#include <iostream>

static GLenum ShaderDataTypeToOpenGLBaseType(Fracture::ShaderDataType type)
{
	switch (type)
	{
	case Fracture::ShaderDataType::Float:    return GL_FLOAT;
	case Fracture::ShaderDataType::Float2:   return GL_FLOAT;
	case Fracture::ShaderDataType::Float3:   return GL_FLOAT;
	case Fracture::ShaderDataType::Float4:   return GL_FLOAT;
	case Fracture::ShaderDataType::Mat3:     return GL_FLOAT;
	case Fracture::ShaderDataType::Mat4:     return GL_FLOAT;
	case Fracture::ShaderDataType::Int:      return GL_INT;
	case Fracture::ShaderDataType::Int2:     return GL_INT;
	case Fracture::ShaderDataType::Int3:     return GL_INT;
	case Fracture::ShaderDataType::Int4:     return GL_INT;
	case Fracture::ShaderDataType::Bool:     return GL_BOOL;
	}

	std::cout<<(false, "Unknown ShaderDataType!")<<std::endl;
	return 0;
}

Fracture::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	bind();
}

Fracture::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void Fracture::VertexArray::bind()
{
	glBindVertexArray(m_Id);
}

void Fracture::VertexArray::unbind()
{
	glBindVertexArray(0);
}

std::shared_ptr<Fracture::VertexArray> Fracture::VertexArray::create()
{
	return std::shared_ptr<VertexArray>(new VertexArray());
}

void Fracture::VertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	bind();
	buffer->bind();
	auto layout = buffer->GetLayout();
	for (auto element : layout)
	{
		switch (element.Type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
		{
			glEnableVertexAttribArray(m_Id);
			glVertexAttribPointer(m_Id,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			m_Id++;
			break;
		}
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			uint8_t count = element.GetComponentCount();
			for (uint8_t i = 0; i < count; i++)
			{
				glEnableVertexAttribArray(m_Id);
				glVertexAttribPointer(m_Id,
					count,
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(element.Offset + sizeof(float) * count * i));
				glVertexAttribDivisor(m_Id, 1);
				m_Id++;
			}
			break;
		}
		default:
		std::cout<< "Unknown ShaderDataType!" <<std::endl;
		}
	}
	m_vertexBuffers.push_back(buffer);
}

void Fracture::VertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> index)
{
	glBindVertexArray(m_Id);
	index->bind();
	m_indexBuffer = index;
}

std::vector<std::shared_ptr<Fracture::VertexBuffer>>& Fracture::VertexArray::getBuffers()
{
	return m_vertexBuffers;
}

std::shared_ptr<Fracture::IndexBuffer>& Fracture::VertexArray::getIndex()
{
	return m_indexBuffer;
}
