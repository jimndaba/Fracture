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
		DepthNode(const std::string& name,const int& width,const int& height, const std::shared_ptr<RenderBucket>& bucket);
		void execute(Renderer& renderer) override;
		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

		float NearPlane = 0.1f;
		float FarPlane = 100.0f;	
	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> outputDepthMap;
	};


}

#endif