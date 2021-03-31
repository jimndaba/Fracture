#pragma once
#ifndef SINKNODE_H
#define SINKNODE_H


#include "FullscreenNode.h"

namespace Fracture
{
	class RenderTarget;
	class Renderer;
	class FrameNodeSerialiser;
	class Shader;
	class FrameResource;

	class SinkNode :public FullScreenNode
	{
	public:
		SinkNode(std::string name);

		SinkNode(std::string name, int width, int height);

		void execute(Renderer& renderer) override;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

		std::shared_ptr<RenderTarget> outputColor;
	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<FrameResource> colorIn;


	};



};


#endif
