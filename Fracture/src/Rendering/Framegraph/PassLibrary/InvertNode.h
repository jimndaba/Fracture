#pragma once
#ifndef INVERTNODE_H
#define INVERTNODE_H

#include "../FullscreenNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;
	class InvertNode : public FullScreenNode
	{
	public: 
		InvertNode(std::string name);

		virtual void execute(Renderer& renderer);

	private:
		std::shared_ptr<Shader> m_invert;
		std::shared_ptr<FrameResource> resource;

	};

}

#endif