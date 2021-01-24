#include "ToneMappingNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
#include "Rendering/Renderer.h"

Fracture::ToneMappingNode::ToneMappingNode(std::string name):FullScreenNode(name)
{
	Gamma = 1.0f;
	Exposure = 1.0f;
	
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("buffer");



	m_toneShader = AssetManager::getShader("ToneMap");
	//Sockets
	AddInputSocket(m_Input);
	//AddInputSocket(m_EnvironmentInput);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, resource);



}

void Fracture::ToneMappingNode::execute(Renderer& renderer)
{	
	resources["buffer"]->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	m_toneShader->use();
	m_toneShader->setFloat("gamma",Gamma);
	m_toneShader->setFloat("exposure",Exposure);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, resources["buffer"]->GetColorTexture(0)->id);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_toneShader->unbind();
	resources["buffer"]->Unbind();
}
