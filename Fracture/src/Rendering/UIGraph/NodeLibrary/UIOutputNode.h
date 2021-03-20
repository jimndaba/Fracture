#pragma once
#ifndef UIOUTPUTNODE_H
#define UIOUTPUTNODE_H

#include "../UINode.h"
#include "Rendering/DrawCommand.h"
#include "Rendering/Framegraph/FullscreenNode.h"
#include "Rendering/Renderer2D/Renderer2D.h"

namespace Fracture
{
	class UISocket;
	class UIMultiSocket;

	template<class T>
	class UIVector;

	class FrameResource;
	class RenderTarget;

	template<class T, typename ...Args>
	class UIFunction;

	class UIOutputNode : public UINode
	{
	public:
		UIOutputNode(Renderer2D& renderer2D,const std::string Name, int width, int height);

		void execute(Renderer& renderer) override;

		void AddSubmit(UISocket input);

		UIMultiSocket Submit;
		UIMultiSocket Submit2D;

		std::vector<DrawCommand> RenderCommands();
		std::shared_ptr<RenderTarget> RenderOut;
	
	private:
		unsigned int quadVAO, quadVBO;

		std::vector<UISocket> m_submitInputs;
		std::vector<UISocket> m_submit2DInputs;

		std::shared_ptr<UIFunction<UIOutputNode>> m_submitFunction;
		std::shared_ptr<UIVector<DrawCommand>> m_GraphCommands;
		Renderer2D& m_2DRenderer;

	};

}

#endif