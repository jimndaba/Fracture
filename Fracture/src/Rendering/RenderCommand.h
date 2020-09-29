#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

namespace Fracture
{

	class RenderCommand
	{
	public:
		RenderCommand()
		{
		
		}
		~RenderCommand()
		{
	
		}	

		unsigned int VAO;
		GLint indiceSize;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Material> material;
		std::vector<std::shared_ptr<Texture>> Textures;		
	};

	class RenderInstancedCommand
	{
	public:
		RenderInstancedCommand(int vao)
		{
			VAO = vao;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glBindVertexArray(0);
		
		}

		~RenderInstancedCommand()
		{}

		unsigned int InstanceID;
		unsigned int VAO;
		std::shared_ptr<TransformComponent> transform;

	};
}

#endif