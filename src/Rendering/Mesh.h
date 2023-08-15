#pragma once
#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include "nlohmann/json.hpp"
#include "Buffer.h"
#include "IGraphicsResource.h"
#include "Animation/AnimationClip.h"
#include "AABB.h"

namespace Fracture
{

	struct SubMesh
	{
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;	
		AABB BoundingBox;
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

	struct Bone
	{
		int ID = 0;
		int ParentID = 0;
		std::string Name = std::string{};	
		glm::mat4 BindTransformation = glm::mat4(1.0f);
		glm::mat4 LocalTransformation = glm::mat4(1.0f);
		glm::mat4 GlobalTransformation = glm::mat4(1.0f);
		glm::mat4 InverseBindTransform = glm::mat4(1.0f);	
	};

	struct StaticMesh : public IGraphicsResource
	{
		StaticMesh(const UUID& id = UUID());
		~StaticMesh();
		UUID ID;
		std::string Name;
		AABB BoundingBox;
		std::vector<SubMesh> SubMeshes;
		std::vector<unsigned int> Indices;
		std::vector<Vertex> mVerticies;
		std::vector<SkinnedVertex> mSkinnedVerticies;
		std::vector<uint32_t> mMaterials;
		std::vector<MeshTriangle> mTriangleCache;
		std::vector<Bone> mBones;	
		std::vector<int> mBoneOrder;
		
		uint32_t VAO;	
		std::shared_ptr<Buffer> VBO_Buffer;
		std::shared_ptr<Buffer> EBO_Buffer;

		glm::mat4 SkeletonInverseTransform;
		glm::mat4 SkeletonSceneTransform;

	private:
		// pure virtual implementation
		void bind() {};
	};


}


#endif