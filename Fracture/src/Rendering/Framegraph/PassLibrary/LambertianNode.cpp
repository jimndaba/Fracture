#include "LambertianNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/RenderBatch.h"

Fracture::LambertianNode::LambertianNode(const std::string& name,const int& width,const int& height, RenderBucket* opaque):
	RenderQueueNode(name)
{

	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	std::shared_ptr<InputSocket> m_EnvInput = std::make_shared<InputSocket>("EnvironmentLight");
	std::shared_ptr<InputSocket> m_DirInput = std::make_shared<InputSocket>("DirectionalShadowMap");
	std::shared_ptr<InputSocket> m_OmniInput = std::make_shared<InputSocket>("OmniShadowMap");

	std::shared_ptr<InputSocket> m_SSAO = std::make_shared<InputSocket>("SSAOMap");

	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputColor");

	//outputColor = std::make_shared<RenderTarget>("Lambertian_out",width, height,TextureTarget::Texture2D, GL_FLOAT, 1,true);
	
	outputColor = RenderTarget::CreateRenderTarget("Lambertian_out", width, height,glAttachmentTarget::Texture2D, FormatType::Float, 1, true);
	
	AcceptBucket(opaque);

	//Sockets
	AddInputSocket(m_Input);
	//AddInputSocket(m_EnvironmentInput);
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
