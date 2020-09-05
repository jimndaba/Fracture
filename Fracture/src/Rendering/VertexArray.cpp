#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <GLAD/glad.h>

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

	//const auto& layout = buffer->GetLayout();

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
