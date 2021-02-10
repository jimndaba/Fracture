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
		SSAONode(const std::string& name,const int& width,const int& height);

		virtual void execute(Renderer& renderer);

		float total_strength = 1.0f;
		float area = 0.002f;
		float falloff = 0.00001f;
		float radius = 0.0025f;

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_blurshader;
		std::shared_ptr<Texture> m_noiseTexture;;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> ssao;
		std::shared_ptr<FrameResource> outputTexture;
		unsigned int noiseTexture;
	};

}

#endif