#include "Model.h"
#include "Mesh.h"
Fracture::Model::Model()
{
}

Fracture::Model::~Model()
{
}

void Fracture::Model::addMesh(std::shared_ptr<Mesh> mesh)
{
	m_meshes.push_back(mesh);
}

std::vector<std::shared_ptr<Fracture::Mesh>> Fracture::Model::GetMeshes()
{
	return m_meshes;
}
