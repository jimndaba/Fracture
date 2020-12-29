#include "Model.h"
#include "Mesh.h"
#include "Logging/Logger.h"

Fracture::Model::Model()
{
}

Fracture::Model::Model(std::string name, std::string path):Name(name),Path(path)
{
}

Fracture::Model::~Model()
{
}

void Fracture::Model::addMesh(std::shared_ptr<Mesh> mesh)
{
	m_meshes.push_back(mesh);
}

std::shared_ptr<Fracture::Mesh> Fracture::Model::GetMesh(std::string name)
{
	for (auto mesh : m_meshes)
	{
		if (mesh->Name == name)
		{
			return mesh;
		}
	}
	FRACTURE_ERROR("Failed to Find Mesh: {}", name);
	return nullptr;
}

std::vector<std::shared_ptr<Fracture::Mesh>> Fracture::Model::GetMeshes()
{
	return m_meshes;
}
