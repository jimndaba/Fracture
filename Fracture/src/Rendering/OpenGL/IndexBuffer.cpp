#include "IndexBuffer.h"
#include "OpenGLBase.h"

Fracture::IndexBuffer::IndexBuffer(void* data, uint32_t size)
{
	glGenBuffers(1, &m_RenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	glNamedBufferStorage(m_RenderID,size,data, GL_MAP_WRITE_BIT);
}

Fracture::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RenderID);
}

void Fracture::IndexBuffer::SetData(void* data, uint32_t offset, uint32_t size)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	glNamedBufferSubData(m_RenderID,offset, size,data);
}

void Fracture::IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
}

std::shared_ptr<Fracture::IndexBuffer> Fracture::IndexBuffer::Create(void* data, uint32_t size)
{
	return std::make_shared<IndexBuffer>(data,size);
}
