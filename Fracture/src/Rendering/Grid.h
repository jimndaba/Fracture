#pragma once

#ifndef GRID_H
#define GRID_H

#include <cstdint>
#include "GLAD/glad.h"

#include <memory>
#include "glm/glm.hpp"
#include <vector>

namespace Fracture
{
	class Shader;
	
	class Grid
	{

	public:
		Grid(int with, int height, float offsetx, float offsety, double thickness);
		~Grid();

		void Draw(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 projection);
		void ReSize(int with, int height, float offsetx, float offsety);

		void SetColor(glm::vec4 color);

	private:
		uint32_t vao, vbo, ibo;
		uint32_t lenght;
		double m_thickness;
		glm::vec4 m_color;
		std::vector<glm::vec3> vertices;
		std::vector<glm::uvec4> indices;

	};
}

#endif