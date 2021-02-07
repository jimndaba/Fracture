#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H


#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>



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


		static std::shared_ptr<Model> loadModel(const std::string &name);
		static std::shared_ptr<Texture> loadTexture(const std::string &name, const std::string &path, Fracture::TextureType texType);

		static const std::shared_ptr<Shader>& getShader(const std::string &name);
		static std::shared_ptr<Model> getModel(const std::string &name);
		static const std::shared_ptr<Material>& getMaterial(const std::string &name);
		static std::shared_ptr<Texture> getTexture(const std::string &name);

		static std::map<std::string, std::shared_ptr<Mesh>> GetMeshes();
		static std::map<std::string, std::shared_ptr<Texture>> GetTextures();
		static std::map<std::string, std::shared_ptr<Model>> GetModels();
		static std::vector<std::shared_ptr<Fracture::Shader>>  GetShaders();
		static std::map<std::string, std::shared_ptr<Material>> GetMaterials();


		static void AddShader(const std::string &name, const std::string& vertex, const std::string& fragment);
		static void AddShader(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geomtry);

		static void AddModel(const std::string& name, const std::string& path);
		static void AddTexture(const std::string& name, const std::string& path, TextureType mtype);
		static void AddTexture(const std::shared_ptr<Texture>& texture);
		static void AddEnvironmentMap(const std::string& name, const std::string& path);
		static void AddMaterial(const std::string& name,const std::shared_ptr<Shader>& shader);
		static void AddMaterial(const std::string& name,const std::shared_ptr<Material>& material);

	
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
		static std::shared_ptr<Fracture::Texture> HDRFromFile(const std::string& name, const char* path,Fracture::TextureType texType, bool gamma = false);
		static std::shared_ptr<Fracture::Material> loadMeshMaterial(aiMaterial* material);



		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles			
			aiProcess_GenSmoothNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |            // Convert UVs if required 			
			aiProcess_ValidateDataStructure|
			aiProcess_RemoveRedundantMaterials
			;   
	};


}

#endif