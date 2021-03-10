#include "LambertianNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"

Fracture::LambertianNode::LambertianNode(const std::string& name,const int& width,const int& height, const std::shared_ptr<RenderBucket>& bucket):
	RenderQueueNode(name,bucket),
	m_width(width),
	m_height(height)
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	

	std::shared_ptr<InputSocket> m_SSAO = std::make_shared<InputSocket>("SSAOMap");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputColor");
	std::shared_ptr<OutputSocket> m_outputNormal = std::make_shared<OutputSocket>("outputNormal");
	std::shared_ptr<OutputSocket> m_graboutput = std::make_shared<OutputSocket>("GrabColor");
	std::shared_ptr<OutputSocket> m_specularout = std::make_shared<OutputSocket>("outputSpecular");
	std::shared_ptr<OutputSocket> m_positionout = std::make_shared<OutputSocket>("outputPosition");
	
	outputColor = RenderTarget::CreateRenderTarget("Lambertian_out", width, height,AttachmentTarget::Texture2D, FormatType::UInt,4, true);
	grabout = RenderTarget::CreateRenderTarget("GrabOut_out", width, height, AttachmentTarget::Texture2D, FormatType::UInt, 1, true);
	normalout = RenderTarget::CreateRenderTarget("Normal_out", width, height, AttachmentTarget::Texture2D, FormatType::UInt, 1, true);
	specularout = RenderTarget::CreateRenderTarget("Specular_out", width, height, AttachmentTarget::Texture2D, FormatType::UInt, 1, true);
	positionout = RenderTarget::CreateRenderTarget("Position_out", width, height, AttachmentTarget::Texture2D, FormatType::UInt, 1, true);
	
	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);
	AddOutputSocket(m_outputNormal);
	AddOutputSocket(m_graboutput);
	AddOutputSocket(m_specularout);
	AddOutputSocket(m_positionout);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddInputResource(m_SSAO, ssaoMap);
	AddOutputResource(m_output, outputColor);

	AddOutputResource(m_outputNormal, normalout);
	AddOutputResource(m_graboutput, grabout);
	AddOutputResource(m_specularout, specularout);
	AddOutputResource(m_positionout, positionout);
}

void Fracture::LambertianNode::execute(Renderer& renderer)
{
	renderer.RenderDirectLightShadows();
	ProfilerTimer timer("Lambertian node");	
	
	resources["outputColor"]->bind();
	renderer.clear();	
	renderOpaque(renderer);
	resources["outputColor"]->Unbind();

	//multitexture grabs
	grabout->blit(resources["outputColor"]->GetBuffer(), m_width, m_height);
	normalout->blit(resources["outputColor"]->GetBuffer(), m_width, m_height,1);
	specularout->blit(resources["outputColor"]->GetBuffer(), m_width, m_height,2);
	positionout->blit(resources["outputColor"]->GetBuffer(), m_width, m_height, 3);
	
	//final passes
	resources["outputColor"]->bind();
	renderTranslusent(renderer);
	renderer.RenderEnvironment();
	renderer.DrawGrid();
	resources["outputColor"]->Unbind();
		
	
}

nlohmann::json Fracture::LambertianNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitLambertianNode(*this);
}

