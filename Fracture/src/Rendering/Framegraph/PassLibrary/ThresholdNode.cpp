#include "ThresholdNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"

Fracture::ThresholdNode::ThresholdNode(const std::string& name,const int& width,const int& height):
	FullScreenNode(name),
	m_shader(AssetManager::getShader("Threshold"))
{

	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("colorTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("thresholdMap");

	outputTexture = std::make_shared<RenderTarget>("Threshold_Out",width, height, TextureTarget::Texture2D, GL_FLOAT, 1,false);

	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddOutputResource(m_output, outputTexture);
}

void Fracture::ThresholdNode::execute(Renderer& renderer)
{
	resources["thresholdMap"]->bind();	
	glBindVertexArray(quadVAO);
	m_shader->use();
	m_shader->setFloat("brightPassThreshold", brightPassThreshold);
	
	m_shader->setTexture("mthreshold",resources["colorTexture"]->GetColorTexture(0).get(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["thresholdMap"]->Unbind();
}
