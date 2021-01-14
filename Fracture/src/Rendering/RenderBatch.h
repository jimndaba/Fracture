#pragma once
#ifndef RENDERBATCH_H
#define RENDERBATCH_H

#include <vector>
#include "RenderCommand.h"

namespace Fracture
{

	class RenderBatch
	{
	public:
		RenderBatch();
		~RenderBatch();

		//each bacth will its own VAO VBO IBO

		std::vector<RenderCommand> m_commnads;
	};

}

#endif