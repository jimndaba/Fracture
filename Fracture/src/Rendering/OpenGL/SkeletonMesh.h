#pragma once
#ifndef SKELETONMESH_H
#define SKELETONMESH_H

#include "Mesh.h"

namespace Fracture
{
	struct AnimatedVertex;
	class Texture2D;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;

	class SkeletonMesh : public Mesh
	{
	public:
		SkeletonMesh(std::vector<AnimatedVertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);

		~SkeletonMesh() = default;

		uint32_t RenderID() const;
		uint32_t CountOfIndices();

		void SetBufferLayout(const VertexBufferLayout& layout);

		void bind();

		static std::shared_ptr<SkeletonMesh> Create(std::vector<AnimatedVertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);

	private:
		//Buffers
		std::shared_ptr<VertexArray> m_VerterArray;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;

		std::vector<AnimatedVertex> m_vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<std::shared_ptr<Texture2D>> m_textures;

	};


}

#endif