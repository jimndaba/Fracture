#pragma once
#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "FrameNode.h"
#include "OutputSocket.h"
#include "GLAD/glad.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Texture.h"
#include "AssetManager/AssetManager.h"
#include "FullscreenNode.h"
#include "Rendering/Renderer.h"

namespace Fracture
{

	class SourceNode :public FrameNode
	{
	public:
		SourceNode(std::string name):FrameNode(name)
		{
			std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("rendertarget");
			AddOutputSocket(m_output);
			AddOutputResource(m_output, resource);
		}

		SourceNode(std::string name,std::shared_ptr<FrameResource> source) :FrameNode(name)
		{
			std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("rendertarget");
			AddOutputSocket(m_output);
			AddOutputResource(m_output, source);
		}

		void execute(Renderer& renderer) override
		{

		}
	private:
		
		std::shared_ptr<FrameResource> resource;

	};

	class SinkNode :public FullScreenNode
	{
	public:
		SinkNode(std::string name) :FullScreenNode(name)
		{
			std::shared_ptr<InputSocket> m_output = std::make_shared<InputSocket>("rendertarget");
			m_shader = AssetManager::getShader("ColorMap");

			AddInputSocket(m_output);
			AddInputResource(m_output,colorIn);
		}

		SinkNode(std::string name, int width, int height) :FullScreenNode(name)
		{
			std::shared_ptr<InputSocket> m_input = std::make_shared<InputSocket>("rendertarget");
			std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("OutputRender");


			m_shader = AssetManager::getShader("ColorMap");
			colorIn = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, false);
			outputColor = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, false);

			AddInputSocket(m_input);
			AddInputResource(m_input,colorIn);

			AddOutputSocket(m_output);
			AddOutputResource(m_output, outputColor);
		}

		void execute(Renderer& renderer) override
		{		
			//renderer.clear();
			glDisable(GL_DEPTH_TEST);
			resources["OutputRender"]->bind();
			m_shader->use();			
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, resources["rendertarget"]->GetColorTexture(0)->id);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);
			m_shader->unbind();
			resources["OutputRender"]->Unbind();
		}
		std::shared_ptr<RenderTarget> outputColor;
	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> colorIn;
	

	};


}

#endif