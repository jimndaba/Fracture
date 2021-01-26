#include "DepthNode.h"

#include "Rendering/RenderTarget.h"

Fracture::DepthNode::DepthNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent):RenderQueueNode(name)
{
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputDepthMap");

	outputDepthMap = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, true);

	AcceptBucket(opaque);
	AcceptBucket(transparent);
	
	AddOutputSocket(m_output);

	AddOutputResource(m_output, outputDepthMap);
}

void Fracture::DepthNode::execute(Renderer& renderer)
{
}
