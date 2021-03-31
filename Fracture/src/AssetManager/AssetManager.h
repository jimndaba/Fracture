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
#include <assimp/pbrmaterial.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Fracture
{
	class Mesh;
	class Skeleton;
	struct BoneInfo;
	struct Bone;
	class Texture;
	class Texture2D;
	class TextureMultiSample;
	class TextureCubeMap;
	class Shader;
	class Model;
	class Material; 
	class AnimationClip;
	class Font;

	class TextureLoader;
	class ShaderLoader;
	class ModelLoader;
	class FontLoader;

	struct ProjectProperties;

	enum class TextureType;

	class AssetManager
	{

	public:
		AssetManager(std::shared_ptr<ProjectProperties> m_properties);
		~AssetManager();

		static std::shared_ptr<Shader>& getShader(const std::string &name);
		static std::shared_ptr<Model> getModel(const std::string &name);
		static const std::shared_ptr<Material>& getMaterial(const std::string &name);
		static std::shared_ptr<Texture2D> getTexture2D(const std::string &name);
		static std::shared_ptr<Texture2D> getHDRTexture(const std::string& name);
		static std::shared_ptr<TextureMultiSample> getMultiSampleTexture(const std::string& name);
		static std::shared_ptr<TextureCubeMap> getCubeMapTexture(const std::string& name);

		static std::shared_ptr<Font> getFont(const std::string& name);

		static std::map<std::string, std::shared_ptr<Mesh>> GetMeshes();
		static std::map<std::string, std::shared_ptr<Texture2D>> GetTextures();
		static std::map<std::string, std::shared_ptr<Texture2D>> GetHDRTextures();
		static std::map<std::string, std::shared_ptr<TextureMultiSample>> GetMultiSampleTextures();
		static std::map<std::string, std::shared_ptr<TextureCubeMap>> GetCubeMapTextures();
		static std::map<std::string, std::shared_ptr<Model>> GetModels();
		static std::vector<std::shared_ptr<Fracture::Shader>>  GetShaders();
		static std::map<std::string, std::shared_ptr<Material>> GetMaterials();

		void Clear();


		static void AddShader(const std::string &name, const std::string& vertex, const std::string& fragment);
		static void AddShader(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geomtry);

		static void AddModel(const std::string& name,const std::shared_ptr<Model>& model);
		static void AddModel(const std::string& name, const std::string& path);		
		
		static void AddTexture2D(const std::string& name, const std::shared_ptr<Texture2D>& texture);
		static void AddTexture2D(const std::string& name, const std::string& path, TextureType mtype);
		static void AddMultiSampleTexture(const std::string& name, const std::string& path, TextureType mtype);
		static void AddHDR(const std::string& name, const std::string& path, TextureType mtype);
		static void AddCubeMap(const std::string& name, const std::string& path, TextureType mtype);

		static void AddFont(const std::string& name, const std::string& path);

		static void AddMaterial(const std::string& name,const std::shared_ptr<Shader>& shader);
		static void AddMaterial(const std::string& name,const std::shared_ptr<Material>& material);

		static std::shared_ptr<ProjectProperties> GetProperties();
	
	private:

		static std::unique_ptr<AssetManager> m_instance;
		static std::shared_ptr<ProjectProperties> m_props;

		static std::unique_ptr<TextureLoader> m_TextureLoader;
		static std::unique_ptr<ShaderLoader> m_ShaderLoader;
		static std::unique_ptr<ModelLoader> m_ModelLoader;
		static std::unique_ptr<FontLoader> m_FontLoader;

		//Libraries
		static std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
		static std::map<std::string, std::shared_ptr<Texture2D>> m_HDRTextures;
		static std::map<std::string, std::shared_ptr<Model>> m_Models;
		static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
		static std::map<std::string, std::shared_ptr<Material>> m_Materials;
		static std::map<std::string, std::shared_ptr<Texture2D>> m_Textures;
		static std::map<std::string, std::shared_ptr<TextureMultiSample>> m_MultiSampleTextures;
		static std::map<std::string, std::shared_ptr<TextureCubeMap>> m_CubeMaps;
		static std::map<std::string, std::shared_ptr<Font>> m_Fonts;
		//Functions
		
	
	};


}

#endif