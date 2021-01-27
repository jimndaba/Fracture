#pragma once
#ifndef SSAONODE_H
#define SSAONODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class Texture;
	class RenderTarget;
	class FrameResource;

	class SSAONode :public FullScreenNode
	{
	public:
		SSAONode(std::string name, int width, int height);

		virtual void execute(Renderer& renderer);

		float total_strength = 1.0;
		float base = 0.5;
		float area = 0.0075;
		float falloff = 0.000001;
		float bias = 0.025;
		float radius = 0.0002;

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Texture> m_noiseTexture;;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputTexture;
		unsigned int noiseTexture;
	};

}

#endif