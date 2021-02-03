#include "IntermediateNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
#include "Rendering/Renderer.h"

Fracture::IntermediateNode::IntermediateNode(std::string name, int width, int height):FrameNode(name)
{
	auto m_Input = std::make_shared<InputSocket>("inputbuffer");
	auto m_output = std::make_shared<OutputSocket>("OutTexture");


	colorOut = std::make_shared<RenderTarget>(width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	//Sockets
	AddInputSocket(m_Input);	
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, inputFBO);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, colorOut);

}

void Fracture::IntermediateNode::execute(Renderer& renderer)
{
	glDisable(GL_DEPTH_TEST);
	resources["OutTexture"]->bind();	
	resources["OutTexture"]->blit(resources["inputbuffer"]->GetID());
	resources["OutTexture"]->Unbind();
}
