#pragma once
#ifndef DEPTHNODE_H
#define DEPTHNODE_H

#include "../RenderQueueNode.h"

namespace Fracture
{
	class Shader;
	class RenderTarget;
	class FrameResource;

	class DepthNode : public RenderQueueNode
	{

	public:
		DepthNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent);
		void execute(Renderer& renderer) override;

		float NearPlane;
		float FarPlane;
	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> outputDepthMap;
	};


}

#endif