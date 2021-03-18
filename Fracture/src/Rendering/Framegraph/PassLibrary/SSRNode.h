#pragma once
#ifndef SSRNODE_H
#define SSRNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class Texture2D;
	class RenderTarget;
	class FrameResource;

	class SSRNode :public FullScreenNode
	{
	public:
		SSRNode(const std::string& name, const int& width, const int& height);

		virtual void execute(Renderer& renderer);

		
		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

	private:
		std::shared_ptr<FrameResource> ssr;
		std::shared_ptr<Shader> m_shader;

		//SpecularMap.
		std::shared_ptr<FrameResource> specular;

		unsigned int noiseTexture;
	};

}

#endif