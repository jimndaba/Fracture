#pragma once
#ifndef THRESHOLDNODE_H
#define THRESHOLDNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;
	class ThresholdNode : public FullScreenNode
	{
	public:
		ThresholdNode(std::string name, int width, int height);

		virtual void execute(Renderer& renderer);

		float brightPassThreshold = 0.4f;

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputTexture;

	};

}

#endif