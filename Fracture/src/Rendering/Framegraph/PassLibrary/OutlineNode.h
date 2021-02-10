#pragma once
#ifndef OUTLINENODE_H
#define OUTLINENODE_H

#include "../RenderQueueNode.h"


namespace Fracture
{
	class Shader;
	class RenderBucket;

	class OutlineNode : public RenderQueueNode
	{
	public:
		OutlineNode(const std::string& name, const int& width, const int& height, RenderBucket* bucket);
		void execute(Renderer& renderer) override;

	private:
		std::shared_ptr<Shader> m_firstshader;
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> in_resource;
		std::shared_ptr<FrameResource> out_resource;
	};

}

#endif 