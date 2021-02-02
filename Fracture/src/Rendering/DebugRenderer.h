#pragma once
#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include "glm/glm.hpp"


namespace Fracture
{

	class DebugRenderer
	{

	public:
		DebugRenderer();
		~DebugRenderer();


		static void DrawGrid(int length,int width,int offset);
		static void DrawLine(glm::vec3 start, glm::vec3 end);

		void Begin();
		void End();

	};

}

#endif