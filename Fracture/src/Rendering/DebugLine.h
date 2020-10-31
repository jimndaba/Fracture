#pragma once
#ifndef DEBUGLINE_H
#define DEBUGLINE_H

#include "glm/glm.hpp"

namespace Fracture
{
	class DebugLine
	{
	private:
		glm::vec3 m_start;
		glm::vec3 m_end;
		glm::vec4 m_color;

		unsigned int VAO;
		unsigned int VBO;

	public:

		DebugLine(glm::vec3 start, glm::vec3 end);
		void Render();


	};



}

#endif