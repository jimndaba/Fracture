#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include <string>


namespace Fracture
{
	class Mesh;

	class Model
	{
	public:
		Model();
		Model(std::string name, std::string path);
		~Model();

		void addMesh(std::shared_ptr<Mesh> mesh);
		void addMaterial(std::string material_name);


		std::string Name;
		std::string Path;
		std::string directory;

		std::shared_ptr<Mesh> GetMesh(std::string name);
		std::vector<std::string> GetMaterials();
		std::vector<std::shared_ptr<Mesh>> GetMeshes();

	private:
		std::vector<std::shared_ptr<Mesh>> m_meshes;
		std::vector<std::string> m_materials;
	};
}
#endif
