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
		ThresholdNode(const std::string& name,const int& width,const int& height);

		virtual void execute(Renderer& renderer);

		float brightPassThreshold = 0.4f;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputTexture;

	};

}

#endif