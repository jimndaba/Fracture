#pragma once
#ifndef ADDIDITIVENODE_H
#define ADDIDITIVENODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;
	class AdditiveMixNode : public FullScreenNode
	{
	public:
		AdditiveMixNode(const std::string& name,const int& width,const int& height);

		virtual void execute(Renderer& renderer);

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> colorA;
		std::shared_ptr<FrameResource> colorB;
		std::shared_ptr<FrameResource> outputColor;

	};

}

#endif