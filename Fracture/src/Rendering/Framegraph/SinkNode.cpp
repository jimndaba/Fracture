#include "SinkNode.h"
#include "OutputSocket.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "AssetManager/AssetManager.h"
#include "FrameNode.h"
#include "Rendering/Renderer.h"

Fracture::SinkNode::SinkNode(std::string name) :FullScreenNode(name),
m_shader(AssetManager::getShader("ColorMap"))
{
	std::shared_ptr<InputSocket> m_output = std::make_shared<InputSocket>("rendertarget");

	AddInputSocket(m_output);
	AddInputResource(m_output, colorIn);
}

Fracture::SinkNode::SinkNode(std::string name, int width, int height) :FullScreenNode(name)
{
	std::shared_ptr<InputSocket> m_input = std::make_shared<InputSocket>("rendertarget");

	colorIn = RenderTarget::CreateRenderTarget("Sink_Color_In", width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);

	outputColor = RenderTarget::CreateRenderTarget("Sink_Color_In", width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);

	m_shader = AssetManager::getShader("ColorMap");

	AddInputSocket(m_input);
	AddInputResource(m_input, colorIn);
	AddResource("OutputRender", outputColor);

}

void Fracture::SinkNode::execute(Renderer& renderer)
{
	//renderer.clear();
	glDisable(GL_DEPTH_TEST);
	resources["OutputRender"]->bind();
	AssetManager::getShader("ColorMap")->use();
	glBindVertexArray(quadVAO);
	AssetManager::getShader("ColorMap")->setTexture("OutMainBuffer", resources["rendertarget"]->GetColorTexture(0).get(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	AssetManager::getShader("ColorMap")->unbind();
	resources["OutputRender"]->Unbind();
}

nlohmann::json Fracture::SinkNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitSinkNode(*this);
}