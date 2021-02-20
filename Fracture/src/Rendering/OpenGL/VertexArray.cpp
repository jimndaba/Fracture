#include "VertexArray.h"
#include "VertexBuffer.h"
#include "OpenGLBase.h"

Fracture::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderID);
}

Fracture::VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_RenderID);
}

void Fracture::VertexArray::bind()
{
	glBindVertexArray(m_RenderID);
}

void Fracture::VertexArray::unbind()
{
	glBindVertexArray(0);
}

void Fracture::VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer>& buffer)
{
	bind();
	//bind buffer and setup Data.
}

uint32_t Fracture::VertexArray::RenderID()
{
	return m_RenderID;
}

std::shared_ptr<Fracture::VertexArray> Fracture::VertexArray::Create()
{
	return std::make_shared<VertexArray>();
}
