#include "VertexBuffer.h"
#include "Vertex.h"
#include "GLAD/glad.h"



Fracture::VertexBuffer::VertexBuffer(unsigned int size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

Fracture::VertexBuffer::VertexBuffer(float* vertices, unsigned int size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Fracture::VertexBuffer::VertexBuffer(std::vector<Vertex> vertices, unsigned int size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);
}

Fracture::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&m_id);
}

void Fracture::VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void Fracture::VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Fracture::VertexBuffer::setData(const void* data, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER,m_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

std::shared_ptr<Fracture::VertexBuffer> Fracture::VertexBuffer::create(unsigned int size)
{
	return std::shared_ptr<VertexBuffer>(new VertexBuffer(size));
}

std::shared_ptr<Fracture::VertexBuffer> Fracture::VertexBuffer::create(std::vector<Vertex> vertices, unsigned int size)
{
	return std::shared_ptr<VertexBuffer>(new VertexBuffer(vertices,size));
}
