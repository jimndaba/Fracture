#include "IndexBuffer.h"
#include "GLAD/glad.h"

Fracture::IndexBuffer::IndexBuffer(unsigned int* indicies, unsigned int count)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indicies, GL_STATIC_DRAW);
}

Fracture::IndexBuffer::IndexBuffer(std::vector<unsigned int> indices, unsigned int count)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

Fracture::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void Fracture::IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void Fracture::IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int Fracture::IndexBuffer::GetCount()
{
	return m_Count;
}
