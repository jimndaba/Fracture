#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include <string>


namespace Fracture
{
	class Mesh;
	class Material; 

	class Model
	{
	public:
		Model();
		Model(std::string name, std::string path);
		~Model();

		void addMesh(std::shared_ptr<Mesh> mesh);
		void addMaterial(std::shared_ptr<Material> material);


		std::string Name;
		std::string Path;
		std::string directory;

		std::shared_ptr<Mesh> GetMesh(std::string name);
		std::vector<std::shared_ptr<Material>> GetMaterials();
		std::vector<std::shared_ptr<Mesh>> GetMeshes();

		std::vector<std::shared_ptr<Material>> m_materials;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}
#endif
