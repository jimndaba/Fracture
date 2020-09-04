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


		void BeginFrame();
		/*
		--Run Render passes:
		--skybox pass
		--light pass
		--Shadow pass
		--Opaque Pass
		--Decal pass
		--Transparency Pass	
		--Effect pass
		*/
		void EndFrame();

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		

	private:

	};

}

#endif