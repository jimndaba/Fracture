#include "AdditiveMixNode.h"
#include "ToneMappingNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"

Fracture::AdditiveMixNode::AdditiveMixNode(std::string name, int width, int height):FullScreenNode(name)
{
	std::shared_ptr<InputSocket> m_InputA = std::make_shared<InputSocket>("colorA");
	std::shared_ptr<InputSocket> m_InputB = std::make_shared<InputSocket>("colorB");

	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("output");

	outputColor = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, false);

	m_shader = AssetManager::getShader("AdditiveMix");

	//Sockets
	AddInputSocket(m_InputA);
	AddInputSocket(m_InputB);
	//Link Sockets to Resources
	AddInputResource(m_InputA, colorA);
	AddInputResource(m_InputB, colorB);

	AddOutputSocket(m_output);
	AddOutputResource(m_output, outputColor);


}

void Fracture::AdditiveMixNode::execute(Renderer& renderer)
{	
	resources["output"]->bind();
	m_shader->use();
	m_shader->setTexture("colorA", resources["colorA"]->GetColorTexture(0).get(), 0);
	m_shader->setTexture("colorB", resources["colorB"]->GetColorTexture(0).get(), 1);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["output"]->Unbind();
}
