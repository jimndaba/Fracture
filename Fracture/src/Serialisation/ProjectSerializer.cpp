#include "ProjectSerializer.h"
#include "SceneSerializer.h"
#include "ProjectProperties.h"
#include "Entity/EntityManager.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/RenderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/LightComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/Texture.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"


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
	for (auto scene = m_scenes.begin(); scene != m_scenes.end(); ++scene)
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

	json materials = json::array_t();
	const auto& m_materials = AssetManager::GetMaterials();
	for (const auto& material : m_materials)
	{		
		json a;
		a["Material Name"] = material.second->Name;
		a["CastShadows"] = material.second->CastShadows();
		a["Transparent"] = material.second->IsTransparent();
		json serialised_unfiorms = json::array_t();
		json serialised_sampleunfiorms = json::array_t();

		const auto& uniforms = material.second->GetUniforms();
		for (auto value = uniforms->begin(); value != uniforms->end(); ++value)
		{
			json b;
			switch (value->second.Type)
			{
			case SHADER_TYPE_BOOL:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Bool;
				break;
			case SHADER_TYPE_INT:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Int;
				break;
			case SHADER_TYPE_FLOAT:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Float;
				break;
			case SHADER_TYPE_VEC2:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec2.x,value->second.Vec2.y };
				break;
			case SHADER_TYPE_VEC3:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec3.x,value->second.Vec3.y,value->second.Vec3.z };
				break;
			case SHADER_TYPE_VEC4:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec4.x,value->second.Vec4.y,value->second.Vec4.z,value->second.Vec4.w };
				break;
			case SHADER_TYPE_COLOR3:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Color3.x,value->second.Color3.y,value->second.Color3.z };
				break;
			case SHADER_TYPE_COLOR4:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Color4.x,value->second.Color4.y,value->second.Color4.z,value->second.Color4.w };
				break;
			case SHADER_TYPE_MAT2:

				break;
			case SHADER_TYPE_MAT3:

				break;
			case SHADER_TYPE_MAT4:

				break;
			default:
				//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
				break;
			}
			serialised_unfiorms.push_back(b);
		}

		const auto& uniformsSamplers = material.second->GetSamplerUniforms();
		for (auto value = uniformsSamplers->begin(); value != uniformsSamplers->end(); ++value)
		{
			json b;
			switch (value->second->Type)
			{
			case SHADER_TYPE_SAMPLER2D:
				b["Type"] = value->second->Type;
				b["Name"] = value->first;
				b["Texture"] = value->second->texture->GetName();
				b["Unit"] = value->second->Unit;
				break;
				FRACTURE_ERROR("Unrecognized Uniform type set");
				break;
			}
			serialised_sampleunfiorms.push_back(b);
		}

		a["MaterialUniforms"] = serialised_unfiorms;
		a["MaterialSampleUniforms"] = serialised_sampleunfiorms;
		a["Shader"] = material.second->getShader()->Name;
		materials.push_back(a);
	}
	j["Materials"] = materials;

	json textures = json::array_t();
	std::map<std::string, std::shared_ptr<Texture2D>> m_textures = AssetManager::GetTextures();
	for(auto texture = m_textures.begin(); texture != m_textures.end(); ++texture)
	{
		json a;
		a["Texture Name"] = texture->first;
		a["Texture Path"] = texture->second->GetPath();
		//a["Texture Type"] = texture->second->GetType();
		textures.push_back(a);
	}
	j["Textures"] = textures;

	json HDRtextures = json::array_t();
	std::map<std::string, std::shared_ptr<Texture2D>> m_HDRtextures = AssetManager::GetHDRTextures();
	for (auto texture = m_HDRtextures.begin(); texture != m_HDRtextures.end(); ++texture)
	{
		json a;
		a["Texture Name"] = texture->first;
		a["Texture Path"] = texture->second->GetPath();
		//a["Texture Type"] = texture->second->GetType();
		HDRtextures.push_back(a);
	}
	j["HDRTextures"] = HDRtextures;
	
	//TO DO Save other textures;

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

	if (exists(input, "HDRTextures"))
	{
		for (auto texture : input["HDRTextures"])
		{
			DeSerializeHDRTextures(texture);
		}
	}

	if (exists(input, "Materials"))
	{
		for (auto material : input["Materials"])
		{
			DeSerializeMaterial(material);
		}
	}

	if (exists(input, "Models"))
	{
		for (auto model : input["Models"])
		{
			DeSerializeModels(model);
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

void Fracture::ProjectSerializer::DeSerializeMaterial(nlohmann::json m)
{
	std::string m_Name = m["Material Name"];
	std::string shader = m["Shader"];
	bool castshadows = m["CastShadows"];
	bool transparent = m["Transparent"];

	std::shared_ptr<Shader> mShader = AssetManager::getShader(shader);
	std::shared_ptr<Material> material = std::make_shared<Material>(m_Name, mShader);

	material->setCastShadows(castshadows);
	material->setIsTransparent(transparent);

	auto uniforms = m["MaterialUniforms"];
	for (auto uniform : uniforms)
	{
		SHADER_TYPE shaderType = (SHADER_TYPE)uniform["Type"];
		switch (shaderType)
		{
		case SHADER_TYPE_BOOL:
		{
			material->setBool(uniform["Name"], (bool)uniform["value"]);
			break;
		}
		case SHADER_TYPE_INT:
		{
			material->setInt(uniform["Name"], (int)uniform["value"]);
			break;
		}
		case SHADER_TYPE_FLOAT:
		{
			material->setFloat(uniform["Name"], (float)uniform["value"]);
			break;
		}
		case SHADER_TYPE_VEC2:
		{
			std::array<float, 2> vec2 = uniform["value"];
			glm::vec2 value = glm::vec2(vec2[0], vec2[1]);
			material->setVec2(uniform["Name"], value);
			break;
		}
		case SHADER_TYPE_VEC3:
		{
			std::array<float, 3> vec3 = uniform["value"];
			glm::vec3 value = glm::vec3(vec3[0], vec3[1], vec3[2]);
			material->setVec3(uniform["Name"], value);
			break;
		}
		case SHADER_TYPE_VEC4:
		{
			std::array<float, 4> vec4 = uniform["value"];
			glm::vec4 value = glm::vec4(vec4[0], vec4[1], vec4[2], vec4[3]);
			material->setVec4(uniform["Name"], value);
			break;
		}
		case SHADER_TYPE_COLOR3:
		{
			std::array<float, 3> color3 = uniform["value"];
			glm::vec3 value = glm::vec3(color3[0], color3[1], color3[2]);
			material->setColor3(uniform["Name"], value);
			break;
		}
		case SHADER_TYPE_COLOR4:
		{
			std::array<float, 4> color4 = uniform["value"];
			glm::vec4 value = glm::vec4(color4[0], color4[1], color4[2], color4[3]);
			material->setColor4(uniform["Name"], value);
			break;
		}
		case SHADER_TYPE_MAT2:

			break;
		case SHADER_TYPE_MAT3:

			break;
		case SHADER_TYPE_MAT4:

			break;
		default:
			//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
			break;
		}

	}

	auto uniformSamples = m["MaterialSampleUniforms"];
	for (auto sample : uniformSamples)
	{
		SHADER_TYPE shaderType = (SHADER_TYPE)sample["Type"];
		switch (shaderType)
		{
		case SHADER_TYPE_SAMPLER2D:
		{
			material->SetTexture(sample["Name"], AssetManager::getTexture2D(sample["Texture"]), (int)sample["Unit"]);
			break;
		}
		FRACTURE_ERROR("Unrecognized Uniform type set");
		break;
		}
	}

	AssetManager::AddMaterial(m_Name, material);
}

void Fracture::ProjectSerializer::DeSerializeTextures(nlohmann::json t)
{
	std::string name = t["Texture Name"];
	std::string path = t["Texture Path"];
	//int _type = t["Texture Type"];
	AssetManager::AddTexture2D(name,path,TextureType::Diffuse);
}

void Fracture::ProjectSerializer::DeSerializeHDRTextures(nlohmann::json t)
{
	std::string name = t["Texture Name"];
	std::string path = t["Texture Path"];
	//int _type = t["Texture Type"];
	AssetManager::AddHDR(name, path, TextureType::Diffuse);
}

void Fracture::ProjectSerializer::DeSerializeScene(nlohmann::json s)
{
	std::string name = s["Scene Name"];
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(name);
	SceneSerializer serializer(scene);
	
	if (!serializer.DeSerialize(m_properties->ScenesPath + "/" + name + ".scene"))
	{
		FRACTURE_ERROR("Failed to Load Scene : {}", name);
	}
	SceneManager::AddScene(name,scene);
}

