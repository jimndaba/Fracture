#include "IntermediateNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Renderer.h"

Fracture::IntermediateNode::IntermediateNode(const std::string& name,const uint32_t& width,const uint32_t& height):
	FrameNode(name),
	m_width(width),
	m_height(height)
{
	auto m_Input = std::make_shared<InputSocket>("inputbuffer");
	auto m_output = std::make_shared<OutputSocket>("OutTexture");

	//colorOut = std::make_shared<RenderTarget>("blit_out",width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	colorOut = RenderTarget::CreateRenderTarget("blit_out", width, height, glAttachmentTarget::Texture2D, FormatType::Float, 1, false);

	//Sockets
	AddInputSocket(m_Input);	
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, inputFBO);
	AddOutputResource(m_output, colorOut);

}

void Fracture::IntermediateNode::execute(Renderer& renderer)
{
	glDisable(GL_DEPTH_TEST);
	resources["OutTexture"]->bind();	
	resources["OutTexture"]->blit(resources["inputbuffer"]->GetBuffer(),m_width,m_height);
	resources["OutTexture"]->Unbind();
}
