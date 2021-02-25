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
		ToneMappingNode(const std::string& name,const int& width,const int& height);

		virtual void execute(Renderer& renderer);

		float Gamma;
		float Exposure;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

	private:
		std::shared_ptr<Shader> m_toneShader;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<RenderTarget> colorOut;

		std::shared_ptr<InputSocket> m_Input;
		std::shared_ptr<OutputSocket> m_output;

	};

}

#endif