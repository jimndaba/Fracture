#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include <string>
#include "AssetManager/IResource.h"

namespace Fracture
{
	class Mesh;


	class Model:public IResource
	{
	public:
		Model();
		Model(std::string name, std::string path);
		~Model();

		void addMesh(std::shared_ptr<Mesh> mesh);

		std::string Name;
		std::string Path;
		std::string directory;

		std::vector<std::shared_ptr<Mesh>> GetMeshes();

	private:
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		
	};
}
#endif
