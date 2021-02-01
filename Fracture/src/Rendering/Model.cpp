#include "Model.h"
#include "Mesh.h"
#include "Logging/Logger.h"
#include "AssetManager/AssetManager.h"

Fracture::Model::Model()
{
}

Fracture::Model::Model(std::string name, std::string path):Name(name),Path(path)
{
}

Fracture::Model::~Model()
{
	m_meshes.clear();
	m_materials.clear();
}

void Fracture::Model::addMesh(std::shared_ptr<Mesh> mesh)
{
	m_meshes.push_back(mesh);
}

void Fracture::Model::addMaterial(std::shared_ptr<Material> material)
{
	m_materials.push_back(material);
}

void Fracture::Model::SetMaterial(const std::string& oldMaterial ,const std::shared_ptr<Material>& material)
{
	for (auto mat : m_materials)
	{
		if (mat->Name == oldMaterial)
		{
			mat = material;
		}

		FRACTURE_INFO("material name now is: {}", mat->Name);
	}
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

std::vector<std::shared_ptr<Fracture::Material>> Fracture::Model::GetMaterials()
{
	return m_materials;
}

std::vector<std::shared_ptr<Fracture::Mesh>> Fracture::Model::GetMeshes()
{
	return m_meshes;
}
