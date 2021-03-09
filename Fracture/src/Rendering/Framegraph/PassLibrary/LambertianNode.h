#pragma once
#ifndef LAMBERTIANFRAME_H
#define LAMBERTIANFRAME_H

#include "../RenderQueueNode.h"

namespace Fracture
{

	class LambertianNode : public RenderQueueNode
	{

	public:
		LambertianNode(const std::string& name,const int& width,const int& height,const std::shared_ptr<RenderBucket>& bucket);
		void execute(Renderer& renderer) override;
		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);
	private:
		uint32_t m_width;
		uint32_t m_height;
		std::shared_ptr<FrameResource> resource;
		std::shared_ptr<FrameResource> ssaoMap;
		std::shared_ptr<FrameResource> outputColor;
		std::shared_ptr<FrameResource> grabout;
	};


}

#endif