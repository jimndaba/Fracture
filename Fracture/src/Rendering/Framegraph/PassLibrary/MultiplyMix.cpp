#include "MultiplyMix.h"
#include "ToneMappingNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Profiling/Profiler.h"

Fracture::MultiplyMixNode::MultiplyMixNode(const std::string& name,const int& width,const int& height):
	FullScreenNode(name),
	m_shader(AssetManager::getShader("MultiplyMix"))
{
	std::shared_ptr<InputSocket> m_InputA = std::make_shared<InputSocket>("colorA");
	std::shared_ptr<InputSocket> m_InputB = std::make_shared<InputSocket>("colorB");

	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("output");

	outputColor = RenderTarget::CreateRenderTarget("Multiply_out",width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);

	//Sockets
	AddInputSocket(m_InputA);
	AddInputSocket(m_InputB);
	//Link Sockets to Resources
	AddInputResource(m_InputA, colorA);
	AddInputResource(m_InputB, colorB);

	AddOutputSocket(m_output);
	AddOutputResource(m_output, outputColor);

}

void Fracture::MultiplyMixNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("Multiply Pass");
	resources["output"]->bind();
	m_shader->use();
	m_shader->setTexture("colorA", resources["colorA"]->GetColorTexture(0).get(), 0);
	m_shader->setTexture("colorB", resources["colorB"]->GetColorTexture(0).get(), 1);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["output"]->Unbind();
}
