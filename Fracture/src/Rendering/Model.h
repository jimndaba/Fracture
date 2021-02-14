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
	class AnimationClip;
	class Skeleton;


	class Model
	{
	public:
		Model();
		Model(std::string name, std::string path);
		~Model();

		void addMesh(std::shared_ptr<Mesh> mesh);
		void addMaterial(std::shared_ptr<Material> material);

		void SetMaterial(const std::string& oldMaterial,const std::shared_ptr<Material>& material);

		std::string Name;
		std::string Path;
		std::string directory;

		std::shared_ptr<Mesh> GetMesh(std::string name);
		std::vector<std::shared_ptr<Material>> GetMaterials();
		std::vector<std::shared_ptr<Mesh>> GetMeshes();

		std::vector<std::shared_ptr<Material>> m_materials;
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		//Animation
		std::vector<std::shared_ptr<AnimationClip>> m_animations;
		std::shared_ptr<Skeleton> m_Skeleton;
		
	};
}
#endif
