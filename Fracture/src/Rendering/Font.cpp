#include "Font.h"
#include "OpenGL/OpenGLBase.h"
#include "Rendering/OpenGL/VertexArray.h"
#include "Rendering/OpenGL/VertexBuffer.h"
#include "Rendering/OpenGL/IndexBuffer.h"

Fracture::Font::Font(const std::string& name, const std::string& path):m_Name(name),m_Path(path)
{
	m_vao = VertexArray::Create();
	m_vao->bind();
	m_vbo = VertexBuffer::Create(NULL, sizeof(float) * 6 * 4,BUFFERUSAGE::DYNAMIC);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_vao->unbind();
}

std::string Fracture::Font::GetName()
{
	return m_Name;
}

std::string Fracture::Font::GetPath()
{
	return m_Path;
}

std::map<char, Fracture::Character> Fracture::Font::GetCharacters()
{
	return m_Characters;
}

void Fracture::Font::AddCharacter(const char c, const Character& cha)
{
	m_Characters.insert(std::pair<char, Character>(c, cha));
}

std::shared_ptr<Fracture::VertexArray> Fracture::Font::GetVAO()
{
	return m_vao;
}

std::shared_ptr<Fracture::VertexBuffer> Fracture::Font::GetBuffer()
{
	return m_vbo;
}

void Fracture::Font::bind()
{
	m_vao->bind();
}

void Fracture::Font::unbind()
{
	m_vao->unbind();
}
