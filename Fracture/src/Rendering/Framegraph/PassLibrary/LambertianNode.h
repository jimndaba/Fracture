#pragma once
#ifndef LAMBERTIANFRAME_H
#define LAMBERTIANFRAME_H

#include "../RenderQueueNode.h"

namespace Fracture
{

	class LambertianNode : public RenderQueueNode
	{

	public:
		LambertianNode(const std::string& name,const int& width,const int& height, std::shared_ptr<RenderBucket> bucket);
		void execute(Renderer& renderer) override;

	private:

		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> ssaoMap;
		std::shared_ptr<FrameResource> outputColor;
	};


}

#endif