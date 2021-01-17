#pragma once
#ifndef RENDERBATCH_H
#define RENDERBATCH_H

#include "GLAD/glad.h"
#include <vector>
#include "DrawCommand.h"

namespace Fracture
{
	enum
	{
		NUM_DRAWS = 1024
	};

	class RenderBatch
	{
	public:
		RenderBatch();
		~RenderBatch();

		void prepare();

		//each bacth will its own VAO VBO IBO


		std::vector<DrawCommand> m_commnads;
		GLuint indirect_draw_buffer;
		GLuint m_Vbo;
		GLuint m_Ibo;
		

	};

}

#endif