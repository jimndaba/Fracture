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
		IntermediateNode(const std::string& name,const int& width,const int& height);

		void execute(Renderer& renderer) override;
	private:
		std::shared_ptr<FrameResource> inputFBO;
		std::shared_ptr<RenderTarget> colorOut;
	};



}

#endif