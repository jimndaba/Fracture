#include "LambertianNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/RenderTarget.h"

Fracture::LambertianNode::LambertianNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent):RenderQueueNode(name)
{

	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	std::shared_ptr<InputSocket> m_EnvInput = std::make_shared<InputSocket>("EnvironmentLight");
	std::shared_ptr<InputSocket> m_DirInput = std::make_shared<InputSocket>("DirectionalShadowMap");
	std::shared_ptr<InputSocket> m_OmniInput = std::make_shared<InputSocket>("OmniShadowMap");

	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputColor");

	outputColor = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, true);

	AcceptBucket(opaque);
	AcceptBucket(transparent);

	//Sockets
	AddInputSocket(m_Input);
	//AddInputSocket(m_EnvironmentInput);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, outputColor);

}

void Fracture::LambertianNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("Lmaertian node");
	renderer.RenderDirectLightShadows();

	resources["outputColor"]->bind();
	renderer.clear();	
	render(renderer);
	renderer.DrawGrid();
	renderer.RenderEnvironment();
	resources["outputColor"]->Unbind();
}
