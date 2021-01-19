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

	struct ProjectProperties;

	enum class TextureType;

	class AssetManager
	{

	public:
		AssetManager(std::shared_ptr<ProjectProperties> m_properties);
		~AssetManager();


		static std::shared_ptr<Model> loadModel(std::string path);
		static std::shared_ptr<Texture> loadTexture(std::string name, std::string path, Fracture::TextureType texType);

		static std::shared_ptr<Shader> getShader(std::string name);
		static std::shared_ptr<Model> getModel(std::string name);
		static std::shared_ptr<Material> getMaterial(std::string name);	
		static std::shared_ptr<Texture> getTexture(std::string name);

		static std::map<std::string, std::shared_ptr<Mesh>> GetMeshes();
		static std::map<std::string, std::shared_ptr<Texture>> GetTextures();
		static std::map<std::string, std::shared_ptr<Model>> GetModels();
		static std::vector<std::shared_ptr<Fracture::Shader>>  GetShaders();
		static std::map<std::string, std::shared_ptr<Material>> GetMaterials();


		static void AddShader(std::string name, std::string vertex, std::string fragment);
		static void AddModel(std::string name, std::string path);
		static void AddTexture(std::string name, std::string path, TextureType mtype);
		static void AddTexture(std::shared_ptr<Texture> texture);
		static void AddEnvironmentMap(std::string name, std::string path);
		static void AddMaterial(std::string name, std::shared_ptr<Shader> shader);
		static void AddMaterial(std::string name, std::shared_ptr<Material> material);

	
		static std::unique_ptr<AssetManager> instance()
		{
			if (!m_instance)
			{
				std::unique_ptr<AssetManager> s_instance (new AssetManager(m_props));
				return s_instance;
			}		
			return nullptr;
		}

	private:
		//static const aiScene* scene;
		//static Assimp::Importer importer;
		static std::unique_ptr<AssetManager> m_instance;
		static std::shared_ptr<ProjectProperties> m_props;
		//Libraries
		static std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
		static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		static std::map<std::string, std::shared_ptr<Model>> m_Models;
		static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
		static std::map<std::string, std::shared_ptr<Material>> m_Materials;
		
		//Functions
		static void ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene);
		static std::shared_ptr<Mesh> processMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiNode* node);
		static void ImportMaterial(aiMaterial* material, std::shared_ptr<Material> f_materail);
		static std::shared_ptr<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, TextureType typeName);
		static std::shared_ptr<Fracture::Texture> TextureFromFile(const char* path, const std::string& directory, Fracture::TextureType texType, bool gamma = false);
		static std::shared_ptr<Fracture::Texture> HDRFromFile(std::string name, const char* path,Fracture::TextureType texType, bool gamma = false);


		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles			
			aiProcess_GenSmoothNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |            // Convert UVs if required 			
			aiProcess_ValidateDataStructure|
			aiProcess_RemoveRedundantMaterials
			;    // Validation
		//aiProcess_OptimizeMeshes |          // Batch draws where possible
		//aiProcess_SortByPType |             // Split meshes by primitive type
	};


}

#endif