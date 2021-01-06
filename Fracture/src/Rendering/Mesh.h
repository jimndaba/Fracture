#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <string>
#include <GLAD/glad.h>
#include "RenderCommand.h"

namespace Fracture
{
	class Vertex;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	struct RenderInstancedElementsCommand;
	class Material;

	class Submesh
	{
	public:
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
		unsigned int IndexCount;

		glm::mat4 Transform;
		std::string MeshName;
	};


	class Mesh
	{

	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
		~Mesh();

		std::string director;		
		unsigned int VAO, VBO, IBO;

		std::string ModelName;
		std::string Name;
		std::string MaterialName;

		//temp
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;


		std::vector<unsigned int> GetIndices()
		{
			return m_indices;
		}

		std::vector<std::shared_ptr<Texture>> Textures()
		{
			return m_textures;
		}

		std::vector<std::shared_ptr<RenderInstancedElementsCommand>> GetInstanceCommands()
		{
			return m_instanceCommands;
		}


		
	private:
		void setupMesh();
		std::vector<Submesh> m_submeshes;
		std::vector<Vertex> m_vertices;
		std::vector<std::shared_ptr<Texture>> m_textures;
		std::vector<std::shared_ptr<RenderInstancedElementsCommand>> m_instanceCommands;
		std::vector<unsigned int> m_indices;


	};

}

#endif
