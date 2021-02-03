#include "BoxBlurNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
#include "Profiling/Profiler.h"

Fracture::BoxBlurNode::BoxBlurNode(std::string name, int width, int height):FullScreenNode(name)
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("colorTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("blurOutput");

	outputTexture = std::make_shared<RenderTarget>(width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	m_shader = AssetManager::getShader("BoxBlur");

	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, outputTexture);
}

void Fracture::BoxBlurNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("BoxBlur Pass");
	resources["blurOutput"]->bind();
	glBindVertexArray(quadVAO);
	m_shader->use();
	m_shader->setFloat("Directions", Directions);
	m_shader->setFloat("Quality", Quality);
	m_shader->setFloat("size", size);
	m_shader->setTexture("boxblur", resources["colorTexture"]->GetColorTexture(0).get(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["blurOutput"]->Unbind();
}
