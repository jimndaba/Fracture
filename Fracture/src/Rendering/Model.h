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

		std::string Name;
		std::string Path;
		std::string directory;

		std::shared_ptr<Mesh> GetMesh(std::string name);

		std::vector<std::shared_ptr<Mesh>> GetMeshes();

		int GetNextMaterialIndex()
		{
			int currentIndex = m_materialIndex;
			m_materialIndex += 1;
			return currentIndex;
		}

	private:
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		int m_materialIndex = 0;
	};
}
#endif
