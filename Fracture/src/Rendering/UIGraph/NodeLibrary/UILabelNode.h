#pragma once
#ifndef UILABELNODE_H
#define UILABELNODE_H

#include "../UINode.h"

namespace Fracture 
{
	class UISocket;
	class UIVec3;
	class UIText;
	class Shader;
	class Renderer;
	class Renderer2D;
	class Font;

	template<class T, typename ...Args>
	class UIFunction;

	class UILabelNode :public UINode
	{
	public:
		UILabelNode(const std::string Name);
		~UILabelNode() = default;

		void execute(Renderer& renderer) override;

		void Submit(Renderer2D& renderer);

		void SetText(const std::string& text);

		UISocket PositionSocket;
		UISocket ScaleSocket;
		UISocket RotationSocket;

		UISocket TextSocket;

		UISocket Exectue;

	private:
		std::shared_ptr<UIText> m_text;
		std::shared_ptr<UIVec3> m_Position;
		std::shared_ptr<UIVec3> m_Scale;
		std::shared_ptr<UIVec3> m_Rotation;

		std::shared_ptr<UIFunction<UILabelNode>> m_execute;
	};

}

#endif