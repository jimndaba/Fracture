#pragma once
#ifndef UISECTORNODE_H
#define UISECTORNODE_H

#include "../UINode.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Rendering/OpenGL/Vertex.h"

namespace Fracture
{
	class UISocket;
	class UIVec3;
	class UIInt;
	class Shader;
	class Renderer;
	class Renderer2D;

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;

	template<class T, typename ...Args>
	class UIFunction;

	class UISectorNode : public UINode
	{
	public:
		UISectorNode(const std::string Name);

		void create();

		void execute(Renderer& renderer) override;

		void Submit(Renderer& renderer);
	
		UISocket PositionSocket;
		UISocket ScaleSocket;
		UISocket RotationSocket;
		UISocket InnerRadiusSocket;
		UISocket OuterRadiusSocket;
		glm::vec3 m_Size;

		UISocket Exectue;

	private:
		std::shared_ptr<Shader> m_shader;
		glm::mat4 m_LocalTransform;

		std::shared_ptr<UIInt> m_InnerCircle;
		std::shared_ptr<UIInt> m_OuterCircle;

		std::shared_ptr<UIVec3> m_Position;
		std::shared_ptr<UIVec3> m_Scale;
		std::shared_ptr<UIVec3> m_Rotation;

		//Buffers
		std::shared_ptr<VertexArray> m_VerterArray;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_Indices;

		std::shared_ptr<UIFunction<UISectorNode>> m_execute;

	};

}

#endif