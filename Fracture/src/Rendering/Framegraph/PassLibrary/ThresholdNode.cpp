#include "ThresholdNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"

Fracture::ThresholdNode::ThresholdNode(std::string name,int width, int height):FullScreenNode(name)
{

	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("colorTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("thresholdMap");

	outputTexture = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1,false);

	m_shader = AssetManager::getShader("Threshold");

	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, outputTexture);
}

void Fracture::ThresholdNode::execute(Renderer& renderer)
{
	resources["thresholdMap"]->bind();	
	m_shader->use();
	m_shader->setTexture("screenTexture",resources["colorTexture"]->GetColorTexture(0).get(), 0);
	glBindVertexArray(quadVAO);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["thresholdMap"]->Unbind();
}
