#pragma once
#ifndef DEPTHNODE_H
#define DEPTHNODE_H

#include "../RenderQueueNode.h"

namespace Fracture
{

	class DepthNode : public RenderQueueNode
	{

	public:
		DepthNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent);
		void execute(Renderer& renderer) override;

	private:

		std::shared_ptr<FrameResource> outputDepthMap;
	};


}

#endif