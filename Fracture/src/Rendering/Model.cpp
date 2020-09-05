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
