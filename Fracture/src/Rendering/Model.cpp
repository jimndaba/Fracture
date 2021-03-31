#include "Model.h"
#include "Material.h"
#include "OpenGL/Mesh.h"
#include "Animation/Skeleton.h"
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
	int newsize = m_materials.size();
	m_materials.resize((material->MaterialCount + 1));
	m_materials[material->MaterialCount] = material;
}

void Fracture::Model::SetMaterial(const std::string& oldMaterial ,const std::shared_ptr<Material>& material)
{
	for (auto& mat : m_materials)
	{
		if (mat->Name == oldMaterial)
		{
			mat = material;
		}
	}
}

std::shared_ptr<Fracture::Mesh> Fracture::Model::GetMesh(std::string name)
{
	for (auto mesh : m_meshes)
	{
		if (mesh->Name() == name)
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

void Fracture::Model::clearMaterials()
{
	m_materials.clear();
}

std::vector<std::shared_ptr<Fracture::Mesh>> Fracture::Model::GetMeshes()
{
	return m_meshes;
}
