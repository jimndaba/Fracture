#include "AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Rendering/OpenGL/Mesh.h"
#include "Rendering/OpenGL/StaticMesh.h"
#include "Rendering/OpenGL/SkeletonMesh.h"
#include "Animation/AnimationClip.h"
#include "Animation/Skeleton.h"
#include "Rendering/Model.h"
#include "TextureLoader.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/Texture2DMultiSample.h"
#include "Rendering/OpenGL/TextureCubeMap.h"
#include "ShaderLoader.h"
#include "ModelLoader.h"
#include "FontLoader.h"
#include "Logging/Logger.h"

#include "Serialisation/ProjectProperties.h"



std::map<std::string, std::shared_ptr<Fracture::Mesh>>  Fracture::AssetManager::m_meshes;
std::map<std::string, std::shared_ptr<Fracture::Texture2D>> Fracture::AssetManager::m_Textures;
std::map<std::string, std::shared_ptr<Fracture::Texture2D>>  Fracture::AssetManager::m_HDRTextures;
std::map<std::string, std::shared_ptr<Fracture::TextureMultiSample>> Fracture::AssetManager::m_MultiSampleTextures;
std::map<std::string, std::shared_ptr<Fracture::TextureCubeMap>> Fracture::AssetManager::m_CubeMaps;
std::map<std::string, std::shared_ptr<Fracture::Model>> Fracture::AssetManager::m_Models;
std::map<std::string, std::shared_ptr<Fracture::Shader>> Fracture::AssetManager::m_Shaders;
std::map<std::string, std::shared_ptr<Fracture::Material>> Fracture::AssetManager::m_Materials;
std::map<std::string, std::shared_ptr<Fracture::Font>> Fracture::AssetManager::m_Fonts;
std::shared_ptr<Fracture::ProjectProperties> Fracture::AssetManager::m_props;

std::unique_ptr<Fracture::TextureLoader> Fracture::AssetManager::m_TextureLoader;
std::unique_ptr<Fracture::ShaderLoader> Fracture::AssetManager::m_ShaderLoader;
std::unique_ptr<Fracture::ModelLoader> Fracture::AssetManager::m_ModelLoader;

Fracture::AssetManager::AssetManager(std::shared_ptr<ProjectProperties> m_properties)
{
	m_props = m_properties;
	FRACTURE_INFO("Asset Manager startup");
}

Fracture::AssetManager::~AssetManager()
{	
	FRACTURE_INFO("Asset Manager Shutdown");
}

void Fracture::AssetManager::AddShader(const std::string& name, const std::string& vertex, const std::string& fragment)
{
	std::shared_ptr<Shader> m_shader = ShaderLoader::LoadShader(name, vertex, fragment);
	m_Shaders[name] = m_shader;
	FRACTURE_TRACE("Loaded Shader: {}", m_shader->Name);
}

void Fracture::AssetManager::AddShader(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geomtry)
{
	std::shared_ptr<Shader> m_shader = ShaderLoader::LoadShader(name, vertex,fragment,geomtry);
	m_Shaders[name] = m_shader;
	FRACTURE_TRACE("Loaded Shader: {}", m_shader->Name);
}

void Fracture::AssetManager::AddModel(const std::string& name, const std::shared_ptr<Model>& model)
{
	m_Models.emplace(name, model);
}

void Fracture::AssetManager::AddModel(const std::string& name, const std::string& path)
{
	std::shared_ptr<Model> model = ModelLoader::LoadModel(name, path);

	if (model)
	{
		model->Name = name;
		model->directory = path;
		m_Models.emplace(name, model);
		FRACTURE_TRACE("Loaded model: {}", name);
	}
	else
	{
		FRACTURE_ERROR("Failed to Load Model: {} ", name);
	}
	
}

void Fracture::AssetManager::AddTexture2D(const std::string& name, const std::shared_ptr<Texture2D>& texture)
{
	m_Textures[name] = texture;
	FRACTURE_TRACE("Loaded Texture: {}", name);
}

void Fracture::AssetManager::AddTexture2D(const std::string& name, const std::string& path,TextureType mtype)
{
	std::shared_ptr<Texture2D> texture = TextureLoader::LoadTexture2D(name,path);	
	m_Textures[name] = texture;
	FRACTURE_TRACE("Loaded Texture: {}",name);
}

void Fracture::AssetManager::AddMultiSampleTexture(const std::string& name, const std::string& path, TextureType mtype)
{
	auto texture = TextureLoader::LoadTextureMultiSample(name, path);
	m_MultiSampleTextures[name] = texture;
}

void Fracture::AssetManager::AddHDR(const std::string& name, const std::string& path, TextureType mtype)
{
	std::shared_ptr<Texture2D> texture = TextureLoader::LoadHDR(name, path);
	
	m_HDRTextures[name] = texture;
	FRACTURE_TRACE("Loaded HDR Texture: {}", name);
}

void Fracture::AssetManager::AddCubeMap(const std::string& name, const std::string& path, TextureType mtype)
{
	std::shared_ptr<TextureCubeMap> texture = TextureLoader::LoadCubeMap(name, path);
	m_CubeMaps[name]= texture;

	FRACTURE_TRACE("No Of Textures: {}", m_Textures.size());
	FRACTURE_TRACE("Loaded Texture: {}", name);
}

void Fracture::AssetManager::AddFont(const std::string& name, const std::string& path)
{
	std::shared_ptr<Font> font = FontLoader::AddFont(name, path);
	m_Fonts[name] = font;
	FRACTURE_TRACE("Loaded font: {}", name);
}

void Fracture::AssetManager::AddMaterial(const std::string& name, const std::shared_ptr<Shader>& shader)
{
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(name,shader));
	m_Materials.emplace(name, material);
	FRACTURE_TRACE("Loaded Material: {}", name);
}

void Fracture::AssetManager::AddMaterial(const std::string& name,const std::shared_ptr<Material>& material)
{
	m_Materials.emplace(name, material);
	FRACTURE_TRACE("Loaded Material: {}", name);
}

std::shared_ptr<Fracture::ProjectProperties> Fracture::AssetManager::GetProperties()
{
	return m_props;
}

const std::shared_ptr<Fracture::Shader>& Fracture::AssetManager::getShader(const std::string& name)
{
	for (auto it = m_Shaders.begin(); it != m_Shaders.end(); ++it)
	{
		if (it->first == name)
			return it->second;
	}
	return nullptr;
}

std::shared_ptr<Fracture::Model> Fracture::AssetManager::getModel(const std::string& name)
{
	return m_Models[name];
}

const std::shared_ptr<Fracture::Material>& Fracture::AssetManager::getMaterial(const std::string& name)
{
	for (auto it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		if (it->first == name)
			return it->second;
	}
	return nullptr;
}

std::shared_ptr<Fracture::TextureMultiSample> Fracture::AssetManager::getMultiSampleTexture(const std::string& name)
{
	return m_MultiSampleTextures[name];
}

std::shared_ptr<Fracture::TextureCubeMap> Fracture::AssetManager::getCubeMapTexture(const std::string& name)
{
	return m_CubeMaps[name];
}

std::shared_ptr<Fracture::Font> Fracture::AssetManager::getFont(const std::string& name)
{
	return m_Fonts[name];
}

std::shared_ptr<Fracture::Texture2D> Fracture::AssetManager::getTexture2D(const std::string& name)
{
	return m_Textures[name];
}

std::shared_ptr<Fracture::Texture2D> Fracture::AssetManager::getHDRTexture(const std::string& name)
{
	return m_HDRTextures[name];
}

std::map<std::string, std::shared_ptr<Fracture::Mesh>> Fracture::AssetManager::GetMeshes()
{
	return m_meshes;
}

std::map<std::string, std::shared_ptr<Fracture::Texture2D>> Fracture::AssetManager::GetTextures()
{
	return m_Textures;
}

std::map<std::string, std::shared_ptr<Fracture::Texture2D>> Fracture::AssetManager::GetHDRTextures()
{
	return m_HDRTextures;
}

std::map<std::string, std::shared_ptr<Fracture::TextureMultiSample>> Fracture::AssetManager::GetMultiSampleTextures()
{
	return m_MultiSampleTextures;
}

std::map<std::string, std::shared_ptr<Fracture::TextureCubeMap>> Fracture::AssetManager::GetCubeMapTextures()
{
	return m_CubeMaps;;
}

std::map<std::string, std::shared_ptr<Fracture::Model>> Fracture::AssetManager::GetModels()
{
	return m_Models;
}

std::vector<std::shared_ptr<Fracture::Shader>> Fracture::AssetManager::GetShaders()
{
	std::vector<std::shared_ptr<Shader>> shaders;

	for (auto shader : m_Shaders)
	{
		shaders.push_back(shader.second);
	}

	return shaders;
}

std::map<std::string, std::shared_ptr<Fracture::Material>> Fracture::AssetManager::GetMaterials()
{
	return m_Materials;
}

void Fracture::AssetManager::Clear()
{
	m_Materials.clear();
	m_meshes.clear();
	m_Shaders.clear();
	m_Models.clear();
	m_Textures.clear();
}

