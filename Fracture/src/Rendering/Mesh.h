#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <string>

namespace Fracture
{
	class Vertex;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	class Mesh
	{

	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
		~Mesh();

		std::string director;
	private:
		void setupMesh();

		std::vector<Vertex> m_vertices;
		std::vector<std::shared_ptr<Texture>> m_textures;
		std::vector<unsigned int> m_indices;

		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
	};

}

#endif
