#pragma once
#ifndef LAMBERTIANFRAME_H
#define LAMBERTIANFRAME_H

#include "../RenderQueueNode.h"

namespace Fracture
{

	class LambertianNode : public RenderQueueNode
	{

	public:
		LambertianNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent);
		void execute(Renderer& renderer) override;

	private:

		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> outputColor;
	};


}

#endif