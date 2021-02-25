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
		OutlineNode(const std::string& name, const int& width, const int& height, const std::shared_ptr<RenderBucket>& bucket);
		void execute(Renderer& renderer) override;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

	private:
		std::shared_ptr<Shader> m_firstshader;
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> in_resource;
		std::shared_ptr<FrameResource> out_resource;
	};

}

#endif 