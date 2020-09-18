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
#include "IResource.h"


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


		template< class T, typename... Args >
		static void AddAsset(Args&&... params);


		std::shared_ptr<Model> loadModel(std::string path);
		std::shared_ptr<Texture> loadTexture(std::string name, std::string path);

		static std::shared_ptr<Shader> getShader(std::string name);
		static std::shared_ptr<Model> getModel(std::string name);
		static std::shared_ptr<Material> getMaterial(std::string name);	

		void AddShader(std::string name, std::string vertex, std::string fragment);
		void AddModel(std::string name, std::string path);
		void AddTexture(std::string name, std::string path);
		void AddMesh(std::string name, std::string path);
		void AddMaterial(std::string name, std::shared_ptr<Shader> shader);

	
		static std::unique_ptr<AssetManager> instance()
		{
			if (!m_instance)
			{
				std::unique_ptr<AssetManager> s_instance (new AssetManager());
				return s_instance;
			}		
			return nullptr;
		}

	private:
		const aiScene* scene = nullptr;
		Assimp::Importer importer;
		static std::unique_ptr<AssetManager> m_instance;
		//Libraries
		static std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
		static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		static std::map<std::string, std::shared_ptr<Model>> m_Models;
		static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
		static std::map<std::string, std::shared_ptr<Material>> m_Materials;
		
		//Functions
		void ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(std::shared_ptr<Model> model, aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	
	};


}

#endif