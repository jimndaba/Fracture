#pragma once
#ifndef MULTIPLYNODE_H
#define MULTIPLYNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;
	class MultiplyMixNode : public FullScreenNode
	{
	public:
		MultiplyMixNode(std::string name, int width, int height);

		virtual void execute(Renderer& renderer);

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> colorA;
		std::shared_ptr<FrameResource> colorB;
		std::shared_ptr<FrameResource> outputColor;

	};

}

#endif