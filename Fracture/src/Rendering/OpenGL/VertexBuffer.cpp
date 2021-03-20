#include "VertexBuffer.h"
#include "OpenGLBase.h"
#include "Vertex.h"
#include <string>

Fracture::VertexBuffer::VertexBuffer(void* data, uint32_t size)
{
	glGenBuffers(1, &m_RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	glNamedBufferData(m_RenderID, size, data, GL_STATIC_DRAW);
}

Fracture::VertexBuffer::VertexBuffer(void* data, uint32_t size, BUFFERUSAGE usage):m_usage(usage)
{
	glGenBuffers(1, &m_RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	glNamedBufferData(m_RenderID, size, data,(GLenum)usage);
}

Fracture::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RenderID);
}

void Fracture::VertexBuffer::SetData(void* data, uint32_t offset, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	//glNamedBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Fracture::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
}

const Fracture::VertexBufferLayout& Fracture::VertexBuffer::GetLayout() const
{
	return m_layout;
}

void Fracture::VertexBuffer::SetLayout(const VertexBufferLayout& layout)
{
	m_layout = layout;
}

uint32_t Fracture::VertexBuffer::GetRenderID()
{
	return m_RenderID;
}

std::shared_ptr<Fracture::VertexBuffer > Fracture::VertexBuffer::Create(void* data, uint32_t size)
{
	return std::make_shared<VertexBuffer>(data,size);
}

std::shared_ptr<Fracture::VertexBuffer > Fracture::VertexBuffer::Create(void* data, uint32_t size,BUFFERUSAGE usage)
{
	return std::make_shared<VertexBuffer>(data, size,usage);
}
