#pragma once
#ifndef INTNODE_H
#define INTNODE_H

#include "../FrameNode.h"

namespace Fracture
{
	class RenderTarget;

	class IntermediateNode : public FrameNode
	{

	public:
		IntermediateNode(const std::string& name,const uint32_t& width,const uint32_t& height);

		void execute(Renderer& renderer) override;
	private:
		std::shared_ptr<FrameResource> inputFBO;
		std::shared_ptr<RenderTarget> colorOut;
		uint32_t m_width;
		uint32_t m_height;
	};



}

#endif