#pragma once
#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include "nlohmann/json.hpp"
#include "Buffer.h"
#include "IGraphicsResource.h"

namespace Fracture
{

	struct SubMesh
	{
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;	
		std::string NodeName, MeshName;
		bool IsVisible;
	};

	struct MeshTriangle
	{
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 v3;
		glm::vec3 Normal;

		float D() const
		{
			// compute d parameter using equation 2
			float d;
			return d = glm::dot(Normal, v1);
		}
	};

	struct StaticMesh : public IGraphicsResource
	{
		StaticMesh(const UUID& id = UUID());
		~StaticMesh();
		UUID ID;
		std::string Name;
		std::vector<SubMesh> SubMeshes;
		std::vector<unsigned int> Indices;
		std::vector<Vertex> mVerticies;
		std::vector<uint32_t> mMaterials;
		
		uint32_t VAO;
		std::shared_ptr<Buffer> Matrix_Buffer;
		std::shared_ptr<Buffer> VBO_Buffer;
		std::shared_ptr<Buffer> EBO_Buffer;

	private:
		// pure virtual implementation
		void bind() {};
	};


	struct SkinnedMesh : public IGraphicsResource
	{
		SkinnedMesh();
		std::vector<SubMesh> SubMeshes;
		std::vector<SkinnedVertex> Verticies;
		std::vector<unsigned int> Indices;
		std::vector<MeshTriangle> TriangleCache;

		uint32_t VAO;

	private:
		// pure virtual implementation
		void bind() {};
	};






}


#endif