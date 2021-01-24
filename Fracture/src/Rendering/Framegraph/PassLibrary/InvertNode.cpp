#include "InvertNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
Fracture::InvertNode::InvertNode(std::string name):FullScreenNode(name)
{

	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("buffer");
	//std::shared_ptr<InputSocket> m_EnvironmentInput = std::make_shared<InputSocket>("environment");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("buffer");
	
	m_invert = AssetManager::getShader("InvertColor");
	//Sockets
	AddInputSocket(m_Input);
	//AddInputSocket(m_EnvironmentInput);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, resource);


}

void Fracture::InvertNode::execute(Renderer& renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	//glClear(GL_COLOR_BUFFER_BIT);
	resources["buffer"]->bind();
	m_invert->use();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D,resources["buffer"]->GetColorTexture(0)->id);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
	m_invert->unbind();
	resources["buffer"]->Unbind();
}
