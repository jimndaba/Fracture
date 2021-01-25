#pragma once
#ifndef BOXBLURNODE_H
#define BOXBLURNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;

	class BoxBlurNode :public FullScreenNode
	{
	public:
		BoxBlurNode(std::string name, int width, int height);

		virtual void execute(Renderer& renderer);

		float Directions = 16.0f;// BLUR DIRECTIONS (Default 16.0 - More is better but slower)
		float Quality = 4.0f; // BLUR QUALITY (Default 4.0 - More is better but slower)
		float size = 8.0f;		

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputTexture;

	};

}

#endif