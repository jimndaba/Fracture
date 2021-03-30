#include "DebugLine.h"
#include "OpenGL/OpenGLBase.h"



Fracture::DebugLine::DebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color):m_start(start),m_end(end),m_color(color)
{
	float lineCoordinates[] = { start.x, start.y, start.z,
							   end.x, end.y,end.z };
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glLineWidth(1.5f);
	glGenBuffers(1, &VBO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineCoordinates), &lineCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float), // stride
		(GLvoid*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Fracture::DebugLine::Render()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

glm::vec4 Fracture::DebugLine::GetColor()
{
	return m_color;
}
