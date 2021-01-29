#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <string>
#include <GLAD/glad.h>
#include "DrawCommand.h"

namespace Fracture
{
	class Vertex;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	struct RenderInstancedElementsCommand;
	class Material;

	struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;

		float Width() const
		{
			return max.x - min.x;
		}

		float Height() const
		{
			return max.y - min.y;
		}

		float Depth() const
		{
			return max.z - min.z;
		}
	};

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
		unsigned int MaterialIndex;

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
		
		void SetAABB(std::shared_ptr<BoundingBox> aabb);
		std::shared_ptr<BoundingBox> GetAABB();

		unsigned int IndexCount;

		
	private:
		void setupMesh();
		std::vector<Submesh> m_submeshes;
		std::vector<Vertex> m_vertices;
		std::vector<std::shared_ptr<Texture>> m_textures;
		std::vector<unsigned int> m_indices;
		std::shared_ptr<BoundingBox> m_bounds;

	};

}

#endif
