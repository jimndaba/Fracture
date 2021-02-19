#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <string>

#include "DrawCommand.h"

namespace Fracture
{
	struct Vertex;
	struct AnimatedVertex;
	class Texture2D;
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
		Mesh(std::vector<Vertex> static_vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures,bool IsAnimated = false);
		Mesh(std::vector<AnimatedVertex> animated_vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, bool IsAnimated = true);
		~Mesh();

		std::string director;		
		unsigned int VAO, VBO, IBO;

		std::string ModelName;
		std::string Name;
		std::string MaterialName;
		unsigned int MaterialIndex;


		std::vector<unsigned int> GetIndices()
		{
			return m_indices;
		}

		std::vector<std::shared_ptr<Texture2D>> Textures()
		{
			return m_textures;
		}
		
		void SetAABB(std::shared_ptr<BoundingBox> aabb);
		std::shared_ptr<BoundingBox> GetAABB();

		unsigned int IndexCount;
		bool m_IsAnimated;
		
	private:
		void setupMesh();
		std::vector<Submesh> m_submeshes;
		std::vector<Vertex> m_StaticVertices;
		std::vector<AnimatedVertex> m_AnimatedVertices;
		std::vector<std::shared_ptr<Texture2D>> m_textures;
		std::vector<unsigned int> m_indices;
		std::shared_ptr<BoundingBox> m_bounds;

	};

}

#endif
