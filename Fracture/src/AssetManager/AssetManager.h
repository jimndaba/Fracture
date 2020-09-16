#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H


#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <string>
#include <GLAD/glad.h>


namespace Fracture
{
	class Mesh;
	class Texture;
	class Shader;
	class Model;
	class Material; 

	class AssetManager
	{

	public:
		AssetManager();
		~AssetManager();

		static std::shared_ptr<Shader> getShader(std::string name);
		static std::shared_ptr<Model> getModel(std::string name);
		static std::shared_ptr<Material> getMaterial(std::string name);

		template< class T, typename... Args >
		static void AddAsset(Args&&... params);

		std::shared_ptr<Model> loadModel(std::string path);
		std::shared_ptr<Texture> loadTexture(std::string path);
		
		void AddShader(std::string name, std::string vertex, std::string fragment);
		void AddModel(std::string name, std::string path);
		void AddTexture(std::string name, std::string path);
		void AddMesh(std::string name, std::string path);
		void AddMaterial(std::string name, std::shared_ptr<Shader> shader);

	private:
		const aiScene* scene = nullptr;
		Assimp::Importer importer;

		//Libraries
		static std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
		static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		static std::map<std::string, std::shared_ptr<Model>> m_Models;
		static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
		static std::map<std::string, std::shared_ptr<Material>> m_Materials;


		std::shared_ptr<Mesh> processMesh(std::shared_ptr<Model> model,aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(std::shared_ptr<Model> model, aiMaterial* mat, aiTextureType type, std::string typeName);
		

		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

		//Functions
		void ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene);
		void ProcessNode(aiNode* node, const aiScene* scene);

	
	};

	template<class T, typename ...Args>
	inline void AssetManager::AddAsset(Args&& ...params)
	{
		auto resource = std::make_shared<T>(params...);

		if (resource)
		{
			std::cout << "added Model";
		}



	}

}

#endif