#include "Grid.h"
#include "Shader.h"
#include <glm\gtc\type_ptr.hpp>

Fracture::Grid::Grid(int width, int height, float offsetx, float offsety, double thickness):m_thickness(thickness)
{
	for (int j = 0; j <= width; ++j) {
		for (int i = 0; i <= height; ++i) {
			float x = ((float)i * (float)offsetx) - (width/2);
			float y = 0;
			float z = ((float)j * (float)offsety) - (height / 2);
			vertices.push_back(glm::vec3(x, y, z));
		}
	}

	for (int j = 0; j < width; ++j) {
		for (int i = 0; i < height; ++i) {

			int row1 = j * (width + 1);
			int row2 = (j + 1) * (height + 1);

			indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
			indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	lenght = (GLuint)indices.size() * 4;

}

Fracture::Grid::~Grid()
{
}

void Fracture::Grid::Draw(std::shared_ptr<Shader> shader,glm::mat4 view,glm::mat4 projection)
{
	glEnable(GL_DEPTH_TEST);
	shader->use();
	shader->setVec4("Color", m_color);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	glLineWidth((GLfloat)m_thickness);
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, lenght, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

}

void Fracture::Grid::ReSize(int width, int height, float offsetx, float offsety)
{
	vertices.clear();
	indices.clear();

	for (int j = 0; j <= width; ++j) {
		for (int i = 0; i <= height; ++i) {
			float x = (float)i / (float)width;
			float y = 0;
			float z = (float)j / (float)height;
			vertices.push_back(glm::vec3(x, y, z));
		}
	}

	for (int j = 0; j < width; ++j) {
		for (int i = 0; i < height; ++i) {

			int row1 = j * (width + 1);
			int row2 = (j + 1) * (height + 1);

			indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
			indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	lenght = (GLuint)indices.size() * 4;

}

void Fracture::Grid::SetColor(glm::vec4 color)
{
	m_color = color;
}
