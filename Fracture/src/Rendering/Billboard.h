#pragma once
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <vector>
#include "OpenGL/Vertex.h"

namespace Fracture
{

	class Billboard
	{

	public:
		Billboard();
		~Billboard();

		unsigned int VAO();
		int Indicies();

	private:
		unsigned int vao, vbo, ibo;
		int m_indiceCount;
		std::vector<Vertex> m_vertices;
	};
	
}

#endif