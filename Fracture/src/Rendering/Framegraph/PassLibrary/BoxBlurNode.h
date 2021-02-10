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
		BoxBlurNode(const std::string& name,const  int& width,const int& height);

		virtual void execute(Renderer& renderer);	
		int amount = 10;
	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_outPutshader;
	
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputTexture;
		std::shared_ptr<RenderTarget> m_blurPasses[2];

	};

}

#endif