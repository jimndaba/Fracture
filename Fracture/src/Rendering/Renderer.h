#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GLAD/glad.h>

namespace Fracture
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		void draw();

	private:

	};

}

#endif