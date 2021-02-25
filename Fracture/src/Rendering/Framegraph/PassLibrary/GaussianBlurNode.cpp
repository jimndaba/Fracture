#include "GaussianBlurNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Profiling/Profiler.h"

Fracture::GaussianBlurNode::GaussianBlurNode(const std::string& name, const int& width, const int& height) :
	FullScreenNode(name),
	m_shader(AssetManager::getShader("BoxBlur")),
	m_outPutshader(AssetManager::getShader("ColorMap"))
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("colorTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("blurOutput");

	//outputTexture = std::make_shared<RenderTarget>("BoxBlur_out", width, height, TextureTarget::Texture2D, GL_FLOAT, 1, false);

	outputTexture = RenderTarget::CreateRenderTarget("BoxBlur_out", width, height, AttachmentTarget::Texture2D,FormatType::Float, 1, false);

	for (int i = 0; i < 2; i++)
	{
		//m_blurPasses[i] = std::make_shared<RenderTarget>("blurPass"+i, width/8, height/8, TextureTarget::Texture2D, GL_FLOAT, 1, false);

		m_blurPasses[i] = RenderTarget::CreateRenderTarget("blurPass" + i, width / 8, height / 8, AttachmentTarget::Texture2D,FormatType::Float, 1, false);
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

void Fracture::GaussianBlurNode::execute(Renderer& renderer)
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
			uint32_t srcWidth = resources["colorTexture"]->GetColorTexture(0)->GetWidth();
			uint32_t srcHeight = resources["colorTexture"]->GetColorTexture(0)->GetHeight();
			uint32_t dstWidth = m_blurPasses[horizontal]->Width;
			uint32_t dstHeight = m_blurPasses[horizontal]->Height;

			m_blurPasses[horizontal]->blit(resources["colorTexture"]->GetBuffer(), srcWidth, srcHeight, dstWidth, dstHeight);
			m_shader->setTexture("boxblur", m_blurPasses[horizontal]->GetColorTexture(0).get(), 0);

			//glBindFramebuffer(GL_READ_FRAMEBUFFER, resources["colorTexture"]->GetID());
			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_blurPasses[horizontal]->GetID());
			//glBlitFramebuffer(0, 0, resources["colorTexture"]->GetColorTexture(0)->width,
				//resources["colorTexture"]->GetColorTexture(0)->height, 0, 0, m_blurPasses[horizontal]->Width, m_blurPasses[horizontal]->Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			
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

	renderer.setViewport(resources["blurOutput"]->GetColorTexture(0)->GetWidth(), resources["blurOutput"]->GetColorTexture(0)->GetHeight());
	{
		uint32_t srcWidth = m_blurPasses[!horizontal]->GetColorTexture(0)->GetWidth();
		uint32_t srcHeight = m_blurPasses[!horizontal]->GetColorTexture(0)->GetHeight();
		uint32_t dstWidth = resources["blurOutput"]->GetColorTexture(0)->GetWidth();
		uint32_t dstHeight = resources["blurOutput"]->GetColorTexture(0)->GetHeight();

		resources["blurOutput"]->blit(m_blurPasses[!horizontal]->GetBuffer(),srcWidth, srcHeight, dstWidth, dstHeight);

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_blurPasses[!horizontal]->GetID());
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resources["blurOutput"]->GetID());
		//glBlitFramebuffer(0, 0, m_blurPasses[!horizontal]->GetColorTexture(0)->width,
		//	m_blurPasses[!horizontal]->GetColorTexture(0)->height, 0, 0, resources["blurOutput"]->GetColorTexture(0)->width, resources["blurOutput"]->GetColorTexture(0)->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		m_outPutshader->use();
		resources["blurOutput"]->bind();
		m_outPutshader->setTexture("OutMainBuffer", resources["blurOutput"]->GetColorTexture(0).get(), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		resources["blurOutput"]->Unbind();
		m_outPutshader->unbind();
	}
}

nlohmann::json Fracture::GaussianBlurNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitGaussianBlurNode(*this);
}
