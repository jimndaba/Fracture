#include "LambertianNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"

Fracture::LambertianNode::LambertianNode(const std::string& name,const int& width,const int& height, const std::shared_ptr<RenderBucket>& bucket):
	RenderQueueNode(name,bucket)
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	std::shared_ptr<InputSocket> m_EnvInput = std::make_shared<InputSocket>("EnvironmentLight");
	std::shared_ptr<InputSocket> m_DirInput = std::make_shared<InputSocket>("DirectionalShadowMap");
	std::shared_ptr<InputSocket> m_OmniInput = std::make_shared<InputSocket>("OmniShadowMap");

	std::shared_ptr<InputSocket> m_SSAO = std::make_shared<InputSocket>("SSAOMap");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputColor");
	
	outputColor = RenderTarget::CreateRenderTarget("Lambertian_out", width, height,AttachmentTarget::Texture2D, FormatType::UInt, 1, true);
	
	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddInputResource(m_SSAO, ssaoMap);
	AddOutputResource(m_output, outputColor);

}

void Fracture::LambertianNode::execute(Renderer& renderer)
{
	renderer.RenderDirectLightShadows();
	ProfilerTimer timer("Lambertian node");	
	resources["outputColor"]->bind();
	renderer.clear();	
	render(renderer);
	renderer.RenderEnvironment();
	renderer.DrawGrid();	
	resources["outputColor"]->Unbind();
}
