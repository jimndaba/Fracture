#include "EnvironmentMapNode.h"
#include "Rendering/Renderer.h"

Fracture::EnvironmentMapNode::EnvironmentMapNode(std::string Name):FrameNode(Name)
{
}

void Fracture::EnvironmentMapNode::execute(Renderer& renderer)
{
	renderer.RenderEnvironment();
}
