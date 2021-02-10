#include "ToneMappingNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
#include "Rendering/Renderer.h"

Fracture::ToneMappingNode::ToneMappingNode(const std::string& name, const int& width, const int& height) :
	FullScreenNode(name),
	Gamma(1.0f),
	Exposure(1.0f)
{
	
	m_Input = std::make_shared<InputSocket>("buffer");
	m_output = std::make_shared<OutputSocket>("colorOut");


	colorOut = std::make_shared<RenderTarget>("ToneMap_ColorOut",width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	m_toneShader = AssetManager::getShader("ToneMap");
	//Sockets
	AddInputSocket(m_Input);
	//AddInputSocket(m_EnvironmentInput);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, colorOut);



}

void Fracture::ToneMappingNode::execute(Renderer& renderer)
{	
	resources["colorOut"]->bind();	
	m_toneShader->use();
	m_toneShader->setFloat("gamma",Gamma);
	m_toneShader->setFloat("exposure",Exposure);
	glBindVertexArray(quadVAO);	
	m_toneShader->setTexture("inputToneImage", resources["buffer"]->GetColorTexture(0).get(),0);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_toneShader->unbind();
	resources["colorOut"]->Unbind();
}
