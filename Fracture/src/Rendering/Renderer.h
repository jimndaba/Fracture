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
		--Run Render passes.
		--Effect passes
		--skybox passes
		--light passes
		--Shadow passes
		--Opaque Pass
		--Transparency Pass	
		*/
		void EndFrame();

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		

	private:

	};

}

#endif