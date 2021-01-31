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
		IntermediateNode(std::string name,int width, int height);

		void execute(Renderer& renderer) override;
	private:
		std::shared_ptr<FrameResource> inputFBO;
		std::shared_ptr<RenderTarget> colorOut;
	};



}

#endif