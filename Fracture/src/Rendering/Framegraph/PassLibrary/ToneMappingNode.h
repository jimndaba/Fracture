#pragma once
#ifndef TONEMAPPINGNODE_H
#define TONEMAPPINGNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;
	class ToneMappingNode : public FullScreenNode
	{
	public:
		ToneMappingNode(std::string name);

		virtual void execute(Renderer& renderer);

		float Gamma;
		float Exposure;

	private:
		std::shared_ptr<Shader> m_toneShader;
		std::shared_ptr<FrameResource> resource;

	};

}

#endif