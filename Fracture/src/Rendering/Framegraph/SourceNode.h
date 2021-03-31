#pragma once
#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "FrameNode.h"
#include "OutputSocket.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/OpenGL/Texture2D.h"
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

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
		{
			return visitor->visitSourceNode(*this);
		}
		
	private:		
		std::shared_ptr<FrameResource> resource;

	};

}

#endif