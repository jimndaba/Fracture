#include "FracturePCH.h"
#include "MeshLoader.h"
#include "Rendering/Mesh.h"
#include "Serialisation/Serialiser.h"
#include "Rendering/GraphicsDevice.h"

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
	mesh->mVerticies.resize(header.VertexCount);
	mesh->Indices.resize(header.IndexCount);
	mesh->mMaterials.resize(header.MaterialCount);

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

	if (fread(mesh->mVerticies.data(), sizeof(Fracture::Vertex),header.VertexCount, f) != header.VertexCount)
	{
		FRACTURE_ERROR("Unable to read vertex data");
		//return nullptr;
	}

	if (fread(mesh->mMaterials.data(), sizeof(uint32_t), header.MaterialCount, f) != header.MaterialCount)
	{
		FRACTURE_ERROR("Unable to read material data");
		//return nullptr;
	}

	for (const auto& mID : mesh->mMaterials)
	{
		FRACTURE_TRACE("Loading Mesh Material : {}", mID);
	}

	fclose(f);
	return mesh;

}

