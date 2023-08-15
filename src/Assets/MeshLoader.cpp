#include "FracturePCH.h"
#include "MeshLoader.h"
#include "Rendering/Mesh.h"
#include "Serialisation/Serialiser.h"
#include "Rendering/GraphicsDevice.h"
#include "Animation/AnimationClip.h"

#include "ImagerLoader.h"

#include <thread>
#include <future>
#include <stdio.h>


Fracture::MeshLoader::MeshLoader()
{
}

std::shared_ptr<Fracture::StaticMesh> Fracture::MeshLoader::LoadStaticMesh(const std::string& path)
{	
	FILE* f = fopen(path.c_str(), "rb");

	assert(f);//File Not converted or present
	if (!f)
	{
		FRACTURE_ERROR("Cannot open {}. Did you forget to imported mesh", path);
		return nullptr;
	}
	
	
	
	MeshFileHeader header;
	if (fread(&header, 1, sizeof(header), f) != sizeof(header))
	{
		FRACTURE_ERROR("Unable to read mesh file header: {}", path);
		return nullptr;
	}
	std::shared_ptr<Fracture::StaticMesh> mesh = std::make_shared<Fracture::StaticMesh>(header.ID);
	
	mesh->SubMeshes.resize(header.SubMeshCount);
	

	if (header.BoneCount)
		mesh->mSkinnedVerticies.resize(header.VertexCount);
	else
		mesh->mVerticies.resize(header.VertexCount);

	mesh->mBones.resize(header.BoneCount);
	mesh->mBoneOrder.resize(header.BoneCount);
	mesh->Indices.resize(header.IndexCount);
	mesh->mMaterials.resize(header.MaterialCount);
	mesh->mTriangleCache.resize(header.MeshTriangleSize);	


	if (fread(mesh->SubMeshes.data(), sizeof(SubMesh), header.SubMeshCount, f) != header.SubMeshCount)
	{
		FRACTURE_ERROR("Could not read sub mesh descriptors");
		return nullptr;
	}

	if (fread(mesh->Indices.data(),sizeof(uint32_t), header.IndexCount, f) != header.IndexCount)
	{
		FRACTURE_ERROR("Unable to read Index data");
		//return nullptr;
	}


	if (header.BoneCount == 0)
	{
		if (fread(mesh->mVerticies.data(), sizeof(Fracture::Vertex), header.VertexCount, f) != header.VertexCount)
		{
			FRACTURE_ERROR("Unable to read vertex data");
			//return nullptr;
		}
	}
	else
	{
		if (fread(mesh->mSkinnedVerticies.data(), sizeof(Fracture::SkinnedVertex), header.VertexCount, f) != header.VertexCount)
		{
			FRACTURE_ERROR("Unable to read skinned vertex data");
			//return nullptr;
		}
	}

	if (fread(mesh->mTriangleCache.data(), sizeof(MeshTriangle), header.MeshTriangleCount, f) != header.MeshTriangleCount)
	{
		FRACTURE_ERROR("Unable to read material data");
		//return nullptr;
	}

	if (fread(mesh->mMaterials.data(), sizeof(uint32_t), header.MaterialCount, f) != header.MaterialCount)
	{
		FRACTURE_ERROR("Unable to read material data");
		//return nullptr;
	}

	if (fread(mesh->mBones.data(), sizeof(Fracture::Bone), header.BoneCount, f) != header.BoneCount)
	{
		FRACTURE_ERROR("Unable to read bone data");
		//return nullptr;
	}	

	if (fread(mesh->mBoneOrder.data(), sizeof(int), header.BoneCount, f) != header.BoneCount)
	{
		FRACTURE_ERROR("Unable to read bone data");
		//return nullptr;
	}
		
	

	for (const auto& submesh : mesh->SubMeshes)
	{
		mesh->BoundingBox = mesh->BoundingBox.Merge(submesh.BoundingBox);
	}




	fclose(f);
	return mesh;

}

