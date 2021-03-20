#pragma once
#ifndef UIBOXNODE_H
#define UIBOXNODE_H

#include "../UINode.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "Rendering/OpenGL/Vertex.h"

namespace Fracture
{
	class UISocket;
	class UIVec3;
	class Shader;
	class Renderer;
	class Renderer2D;


	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;

	template<class T, typename ...Args>
	class UIFunction;
	
	class UIBoxNode : public UINode
	{
	public:
		UIBoxNode(const std::string Name);

		void execute(Renderer& renderer) override;

		void Submit(Renderer& renderer);

		UISocket PositionSocket;
		UISocket ScaleSocket;
		UISocket RotationSocket;		
		
		UISocket Exectue;

		void UpdateTransforms();

		void create();
	
	private:
		std::shared_ptr<Shader> m_shader;
		unsigned int quadVAO, quadVBO;
		glm::mat4 m_LocalTransform;
		std::shared_ptr<UIVec3> m_Position;
		std::shared_ptr<UIVec3> m_Scale;
		std::shared_ptr<UIVec3> m_Rotation;
		glm::vec3 m_Size;

		//Buffers
		std::shared_ptr<VertexArray> m_VerterArray;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_Indices;

		std::shared_ptr<UIFunction<UIBoxNode>> m_execute;

	};

}

#endif