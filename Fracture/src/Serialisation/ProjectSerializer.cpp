#include "ProjectSerializer.h"
#include "Scene/SceneManager.h"
#include "AssetManager/AssetManager.h"
#include "SceneSerializer.h"
#include "ProjectProperties.h"


Fracture::ProjectSerializer::ProjectSerializer(std::shared_ptr<ProjectProperties> properties):m_properties(properties)
{
}

Fracture::ProjectSerializer::~ProjectSerializer()
{
}

void Fracture::ProjectSerializer::Serialize(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["Project"] = m_properties->ProjectName;
	j["IsMaximised"] = true;

	j["Game Config Path"] = m_properties->GameConfigPath;
	j["Content Path"] = m_properties->ContentDirectory;
	j["Textures Path"] = m_properties->TexturesPath;
	j["Shaders Path"] = m_properties->ShadersPath;
	j["Models Path"] = m_properties->ModelsPath;
	j["Scenes Path"] = m_properties->ScenesPath;
	j["Active Scene"] = SceneManager::GetActiveScene()->Name;

	json scenes = json::array_t();
	std::map<std::string, std::shared_ptr<Scene>> m_scenes = SceneManager::GetScenes();
	for(auto scene = m_scenes.begin(); scene != m_scenes.end(); ++scene)
	{
		json a;
		a["Scene Name"] = scene->first;
		SceneSerializer serializer(scene->second);
		serializer.Serialize(m_properties->ScenesPath + "/" + scene->first + ".scene");
		scenes.push_back(a);
	}
	j["Scenes"] = scenes;


	json models = json::array_t();
	std::map<std::string, std::shared_ptr<Model>> m_models = AssetManager::GetModels();
	for (auto model = m_models.begin(); model != m_models.end(); ++model)
	{
		json a;
		a["Model Name"] = model->first;
		a["Model Path"] = model->second->Path;
		a["Model Directory"] = model->second->directory;
		models.push_back(a);
	}
	j["Models"] = models;

	json shaders = json::array_t();
	std::vector<std::shared_ptr<Shader>> m_shaders = AssetManager::GetShaders();
	for (auto shader : m_shaders)
	{
		json a;
		a["Shader Name"] = shader->Name;
		a["Vertex"] = shader->vertPath;
		a["Fragment"] = shader->fragPath;
		shaders.push_back(a);
	}
	j["Shaders"] = shaders;

	json textures = json::array_t();
	std::map<std::string, std::shared_ptr<Texture>> m_textures = AssetManager::GetTextures();
	for(auto texture = m_textures.begin(); texture != m_textures.end(); ++texture)
	{
		json a;
		a["Texture Name"] = texture->first;
		a["Texture Path"] = texture->second->path;
		a["Texture Type"] = texture->second->textureType;
		textures.push_back(a);
	}
	j["Textures"] = textures;
	

	j["Physics Settings"] = "------------------------------------";
	j["Gravity"] = "PhysicsSettings";

	j["Render Settings"] = "------------------------------------";
	j["DrawDebug"] = false;
	j["DrawShadows"] = true;

	   
	m_OutputStream << j;
	m_OutputStream.close();
}

bool Fracture::ProjectSerializer::DeSerialize(const std::string& filepath)
{
	std::ifstream stream(filepath);
	json input;

	if (!stream.good())
	{
		FRACTURE_ERROR("Can't read file");
		return false;
	}

	stream >> input;
	if (input.is_null())
	{
		FRACTURE_ERROR("File is either non-json file or corrupt;");
		return false;
	}

	
	m_properties->ProjectName = input["Project"];
	m_properties->GameConfigPath = input["Game Config Path"];
	m_properties->TexturesPath = input["Textures Path"];
	m_properties->ShadersPath = input["Shaders Path"];
	m_properties->ModelsPath = input["Models Path"];
	m_properties->ScenesPath = input["Scenes Path"];
	m_properties->ActiveScene = input["Active Scene"];

	if (exists(input, "Models"))
	{
		for (auto model : input["Models"])
		{
			DeSerializeModels(model);			
		}
	}
	if (exists(input, "Shaders"))
	{
		for (auto shader : input["Shaders"])
		{
			DeSerializeShaders(shader);
		}
	}
	if (exists(input, "Textures"))
	{
		for (auto texture : input["Textures"])
		{
			DeSerializeTextures(texture);
		}
	}
	if (exists(input, "Scenes"))
	{
		for (auto scene: input["Scenes"])
		{
			DeSerializeScene(scene);
		}
	}

	

	return true;
}

void Fracture::ProjectSerializer::DeSerializeModels(nlohmann::json m)
{
	std::string name = m["Model Name"];
	std::string path = m["Model Directory"];
	AssetManager::AddModel(name, path);
}

void Fracture::ProjectSerializer::DeSerializeShaders(nlohmann::json s)
{
	std::string name = s["Shader Name"];
	std::string vertex= s["Vertex"];;
	std::string fragment= s["Fragment"];
	AssetManager::AddShader(name, vertex, fragment);
}

void Fracture::ProjectSerializer::DeSerializeTextures(nlohmann::json t)
{
	std::string name = t["Texture Name"];
	std::string path = t["Texture Path"];
	int _type = t["Texture Type"];
	AssetManager::AddTexture(name, path, (TextureType)_type);
}

void Fracture::ProjectSerializer::DeSerializeScene(nlohmann::json s)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	SceneSerializer serializer(scene);
	std::string name = s["Scene Name"];
	if (!serializer.DeSerialize(m_properties->ScenesPath + "/" + name + ".scene"))
	{
		FRACTURE_ERROR("Failed to Load Scene : {}", name);
	}
	SceneManager::AddScene(name,scene);
}

