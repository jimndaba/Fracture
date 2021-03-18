#pragma once
#ifndef UISETPOS_H
#define UISETPOS_H

#include "../UINode.h"
#include "Rendering/DrawCommand.h"
#include "Rendering/Framegraph/FullscreenNode.h"

namespace Fracture
{
	class UISocket;
	class UIVec3;

	class UISetPositionNode : public UINode
	{
	public:
		UISetPositionNode(const std::string Name);

		void execute(Renderer& renderer) override;

		void SetPosition(const glm::vec3& position);

		UISocket Output;	

	private:
		std::shared_ptr<UIVec3> m_Position;
	
		
	};

}

#endif