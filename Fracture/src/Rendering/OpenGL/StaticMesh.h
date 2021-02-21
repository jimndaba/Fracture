#pragma once
#ifndef STATICMESH_H
#define STATICMESH_H

#include "Mesh.h"

namespace Fracture
{
	struct Vertex;
	class Texture2D;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;

	class StaticMesh : public Mesh
	{
	public: 
		StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
		~StaticMesh() = default;

		uint32_t RenderID() const;
		uint32_t CountOfIndices();

		void SetBufferLayout(const VertexBufferLayout& layout);

		void bind();

		static std::shared_ptr<StaticMesh> Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);

	private:
		//Buffers
		std::shared_ptr<VertexArray> m_VerterArray;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<std::shared_ptr<Texture2D>> m_textures;
	};


}

#endif