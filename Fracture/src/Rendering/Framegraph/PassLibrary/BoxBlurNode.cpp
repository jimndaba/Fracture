#include "BoxBlurNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Profiling/Profiler.h"

Fracture::BoxBlurNode::BoxBlurNode(const std::string& name, const int& width, const int& height) :
	FullScreenNode(name),
	m_shader(AssetManager::getShader("BoxBlur")),
	m_outPutshader(AssetManager::getShader("ColorMap"))
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("colorTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("blurOutput");

	outputTexture = std::make_shared<RenderTarget>("BoxBlur_out", width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	for (int i = 0; i < 2; i++)
	{
		m_blurPasses[i] = std::make_shared<RenderTarget>("blurPass"+i, width/8, height/8, TextureTarget::Texture2D, GL_FLOAT, 1, false);
		m_blurPasses[i]->SetResizable(false);
		AddResource("blurPass" + i, m_blurPasses[i]);
	}
	
	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddOutputResource(m_output, outputTexture);

	
}

void Fracture::BoxBlurNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("BoxBlur Pass");
	bool horizontal = true, first_iteration = true;	

	glBindVertexArray(quadVAO);
	m_shader->use();	
	for (int i = 0; i < amount; i++)
	{
		renderer.setViewport(m_blurPasses[horizontal]->Width, m_blurPasses[horizontal]->Height);
		m_blurPasses[horizontal]->bind();
		m_shader->setInt("horizontal", horizontal);
	
		if (first_iteration)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, resources["colorTexture"]->GetID());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_blurPasses[horizontal]->GetID());
			glBlitFramebuffer(0, 0, resources["colorTexture"]->GetColorTexture(0)->width,
				resources["colorTexture"]->GetColorTexture(0)->height, 0, 0, m_blurPasses[horizontal]->Width, m_blurPasses[horizontal]->Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			m_shader->setTexture("boxblur", m_blurPasses[horizontal]->GetColorTexture(0).get(), 0);	
		}
		else
		{
			m_shader->setTexture("boxblur", m_blurPasses[!horizontal]->GetColorTexture(0).get(), 0);
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	m_shader->unbind();

	renderer.setViewport(resources["blurOutput"]->GetColorTexture(0)->width, resources["blurOutput"]->GetColorTexture(0)->height);
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_blurPasses[!horizontal]->GetID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resources["blurOutput"]->GetID());
		glBlitFramebuffer(0, 0, m_blurPasses[!horizontal]->GetColorTexture(0)->width,
			m_blurPasses[!horizontal]->GetColorTexture(0)->height, 0, 0, resources["blurOutput"]->GetColorTexture(0)->width, resources["blurOutput"]->GetColorTexture(0)->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		m_outPutshader->use();
		resources["blurOutput"]->bind();
		m_outPutshader->setTexture("OutMainBuffer", resources["blurOutput"]->GetColorTexture(0).get(), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		resources["blurOutput"]->Unbind();
		m_outPutshader->unbind();
	}
}
