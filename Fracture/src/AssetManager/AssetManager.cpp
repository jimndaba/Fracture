#include "AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Model.h"
#include "Rendering/Texture.h"
#include "Rendering/Vertex.h"
#include "Logging/Logger.h"
#include "Math/Math.h"
#include "Serialisation/ProjectProperties.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include <assimp/pbrmaterial.h>

std::unique_ptr<Fracture::AssetManager> Fracture::AssetManager::m_instance;

std::map<std::string, std::shared_ptr<Fracture::Mesh>>  Fracture::AssetManager::m_meshes;
std::map<std::string, std::shared_ptr<Fracture::Texture>> Fracture::AssetManager::m_Textures;
std::map<std::string, std::shared_ptr<Fracture::Model>> Fracture::AssetManager::m_Models;
std::map<std::string, std::shared_ptr<Fracture::Shader>> Fracture::AssetManager::m_Shaders;
std::map<std::string, std::shared_ptr<Fracture::Material>> Fracture::AssetManager::m_Materials;
std::shared_ptr<Fracture::ProjectProperties> Fracture::AssetManager::m_props;

Fracture::AssetManager::AssetManager(std::shared_ptr<ProjectProperties> m_properties)
{
	m_props = m_properties;
}

Fracture::AssetManager::~AssetManager()
{
	m_Materials.clear();
	m_meshes.clear();
	m_Shaders.clear();
	m_Models.clear();
	m_Textures.clear();
}

void Fracture::AssetManager::AddShader(std::string name, std::string vertex, std::string fragment)
{
	std::shared_ptr<Shader> m_shader = std::shared_ptr<Shader>(new Shader(name,vertex, fragment));	
	m_Shaders.emplace(name, m_shader);
	FRACTURE_TRACE("Loaded Shader: {}", name);
}

void Fracture::AssetManager::AddModel(std::string name, std::string path)
{
	std::shared_ptr<Model> model = loadModel(path);

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

void Fracture::AssetManager::AddTexture(std::string name, std::string path, TextureType mtype)
{
	std::shared_ptr<Texture> texture = loadTexture(name,path,mtype);
	m_Textures.emplace(name, texture);
	FRACTURE_TRACE("Loaded Texture: {}",name);
}

void Fracture::AssetManager::AddTexture(std::shared_ptr<Texture> texture)
{
	m_Textures.emplace(texture->Name, texture);
	FRACTURE_TRACE("Loaded Texture: {}", texture->Name);
}

void Fracture::AssetManager::AddEnvironmentMap(std::string name, std::string path)
{
	std::shared_ptr<Texture> texture = HDRFromFile(name,path.c_str(),TextureType::Environment);
	m_Textures.emplace(name, texture);
	FRACTURE_TRACE("Loaded HDR Environment: {}", name);
}

void Fracture::AssetManager::AddMaterial(std::string name, std::shared_ptr<Shader> shader)
{
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(name,shader));
	m_Materials.emplace(name, material);
	FRACTURE_TRACE("Loaded Material: {}", name);
}

void Fracture::AssetManager::AddMaterial(std::string name, std::shared_ptr<Material> material)
{
	m_Materials.emplace(name, material);
	FRACTURE_TRACE("Loaded Material: {}", name);
}

std::shared_ptr<Fracture::Shader> Fracture::AssetManager::getShader(std::string name)
{
	return m_Shaders[name];
}

std::shared_ptr<Fracture::Model> Fracture::AssetManager::getModel(std::string name)
{
	return m_Models[name];
}

std::shared_ptr<Fracture::Material> Fracture::AssetManager::getMaterial(std::string name)
{
	return m_Materials[name];
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::getTexture(std::string name)
{
	return m_Textures[name];
}

std::map<std::string, std::shared_ptr<Fracture::Mesh>> Fracture::AssetManager::GetMeshes()
{
	return m_meshes;
}

std::map<std::string, std::shared_ptr<Fracture::Texture>> Fracture::AssetManager::GetTextures()
{
	return m_Textures;
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

std::shared_ptr<Fracture::Model> Fracture::AssetManager::loadModel(std::string path)
{
	std::shared_ptr<Model> m_model = nullptr;
	Assimp::Importer importer;
	

	const aiScene* scene = importer.ReadFile(path, s_MeshImportFlags);//| aiProcess_CalcTangentSpace
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		FRACTURE_ERROR("ERROR::ASSIMP:: {}", importer.GetErrorString());	
		return 0;
	}

	m_model = std::shared_ptr<Model>(new Model());
	m_model->Name = scene->GetShortFilename(path.c_str());
	m_model->directory = path.substr(0, path.find_last_of('\\'));
	
	// process ASSIMP's root node recursively
	
	ProcessNode(m_model, scene->mRootNode, scene);

	return m_model;
}

std::shared_ptr<Fracture::Mesh> Fracture::AssetManager::processMesh(std::shared_ptr<Model> model,aiMesh* mesh, const aiScene* scene, aiNode* node)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// process vertex positions, normals and texture coordinates
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uvs = vec;
		}
		else
		{
			vertex.uvs = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			//Tangents
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	
	aiMaterial* currentMaterial(scene->mMaterials[mesh->mMaterialIndex]);	
	std::string mesh_name = node->mName.data;
	std::string mat_name;

	std::string temp = node->mName.data;
	mat_name = temp + ".material_" + std::to_string(mesh->mMaterialIndex);

	std::shared_ptr<Material> m_material = std::shared_ptr<Material>(new Material(mat_name, getShader("PBRTexturedShader")));

	ImportMaterial(currentMaterial, m_material);

	AddMaterial(mat_name, m_material);	

	std::shared_ptr<Mesh> new_mesh = std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures));

	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 position; 

	aiMatrix4x4 transform = node->mTransformation;
	Math::DecomposeTransform(Math::Mat4FromAssimpMat4(transform), position, rotation, scale);
	new_mesh->MaterialName = mat_name;
	new_mesh->position = position;
	new_mesh->scale = scale;
	new_mesh->rotation =rotation;
	new_mesh->Name = mesh_name;
	new_mesh->ModelName = model->Name;

	std::shared_ptr<BoundingBox> aabb = std::make_shared<BoundingBox>();
	//process aabb
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		aabb->min.x = glm::min(vertex.position.x, aabb->min.x);
		aabb->min.y = glm::min(vertex.position.y, aabb->min.y);
		aabb->min.z = glm::min(vertex.position.z, aabb->min.z);
		aabb->max.x = glm::max(vertex.position.x, aabb->max.x);
		aabb->max.y = glm::max(vertex.position.y, aabb->max.y);
		aabb->max.z = glm::max(vertex.position.z, aabb->max.z);
	}

	new_mesh->SetAABB(aabb);

	return new_mesh;
}

//Process each material from Mesh
void Fracture::AssetManager::ImportMaterial(aiMaterial* material, std::shared_ptr<Material> f_materail)
{
	if (material == nullptr)
	{
		return;
	}

	aiString aiTexPath;
	aiColor3D aiColor;
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) != aiReturn_SUCCESS)
		aiColor = aiColor3D{ 1.0f,1.0f,1.0f };

	float shininess, metalness;
	if (material->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
		shininess = 80.0f; // Default value

	if (material->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
		metalness = 0.0f;


	float roughness = 1.0f - glm::sqrt(shininess / 100.0f);

	// process material
	// 1. diffuse maps	
	if (material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS)
	{
		std::shared_ptr<Texture> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, TextureType::Diffuse);

		if (texture)
		{
			f_materail->setFloat("albedoFlag", 1.0f);
			f_materail->SetTexture("albedoMap", texture, 3);
			if (texture->Format == GL_RGBA)
			{
				f_materail->setIsTransparent(true);
				f_materail->setFloat("TransparencyFlag", 1.0f);
			}
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
			// Fallback to albedo color
			f_materail->setFloat("TransparencyFlag", 0.0f);
			f_materail->setFloat("albedoFlag", 0.0f);
			f_materail->setColor3("u_albedo", glm::vec3(aiColor.r, aiColor.g, aiColor.b));
		}
	}
	else
	{
		f_materail->setFloat("albedoFlag", 0.0f);
		f_materail->setFloat("TransparencyFlag", 0.0f);
		f_materail->setColor3("u_albedo", glm::vec3(aiColor.r, aiColor.g, aiColor.b));
	}

	// 2. normal maps
	if (material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS)
	{
		std::shared_ptr<Texture> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_NORMALS, TextureType::Normal);
		if (texture)
		{
			f_materail->SetTexture("normalMap", texture, 4);
			f_materail->setFloat("normalFlag", 1.0f);
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
			f_materail->setFloat("normalFlag", 0.0f);
			// Fallback to albedo color				
		}
	}
	else
	{
		f_materail->setFloat("normalFlag", 0.0f);
	}

	// 3. Roughness map
	if (material->GetTexture(aiTextureType::aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS)
	{
		std::shared_ptr<Texture> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_SHININESS, TextureType::Roughness);
		if (texture)
		{
			f_materail->setFloat("roughnessFlag", 1.0f);
			f_materail->SetTexture("roughnessMap", texture, 5);
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());

			f_materail->setFloat("roughnessFlag", 0.0f);
			f_materail->setFloat("u_roughness", roughness);

			// Fallback to albedo color				
		}
	}
	else
	{
		f_materail->setFloat("roughnessFlag", 0.0f);
		f_materail->setFloat("u_roughness", roughness);
	}

	// 1. Metallic map
	if (material->GetTexture(aiTextureType::aiTextureType_METALNESS, 0, &aiTexPath) == AI_SUCCESS)
	{
		std::shared_ptr<Texture> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_METALNESS, TextureType::Metallic);
		if (texture)
		{
			f_materail->setFloat("metallicFlag", 1.0f);
			f_materail->SetTexture("metallicMap", texture, 6);
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
			f_materail->setFloat("metallicFlag", 0.0f);
			f_materail->setFloat("u_metallic", metalness);
			// Fallback to albedo color				
		}
	}
	else
	{
		f_materail->setFloat("metallicFlag", 0.0f);
		f_materail->setFloat("u_metallic", metalness);
	}

	// 1. ao map
	if (material->GetTexture(aiTextureType::aiTextureType_AMBIENT_OCCLUSION, 0, &aiTexPath) == AI_SUCCESS)
	{
		std::shared_ptr<Texture> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_AMBIENT_OCCLUSION, TextureType::AO);
		if (texture)
		{
			f_materail->setFloat("aoFlag", 1.0f);
			f_materail->SetTexture("aoMap", texture, 7);
			//mi->Set("u_AlbedoTexToggle", 1.0f); - way to toggle texture on or off
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
			f_materail->setFloat("aoFlag", 0.0f);
			f_materail->setFloat("u_ao", 1.0f);
			// Fallback to albedo color				
		}
	}
	else
	{
		f_materail->setFloat("aoFlag", 0.0f);
		f_materail->setFloat("u_ao", 1.0f);
	}
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::loadMaterialTexture(aiMaterial* mat, aiTextureType type, Fracture::TextureType typeName)
{
	std::shared_ptr<Texture> texture;

	aiString str;
	mat->GetTexture(type, 0,&str);
	bool skip = false;

	for (unsigned int j = 0; j < m_Textures.size(); j++)
	{
		for (const auto& component_pair : m_Textures)
		{
			if (std::strcmp(component_pair.first.c_str(), str.C_Str()) == 0)
			{			
				skip = true;
				return getTexture(str.C_Str());		
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it		
			texture = TextureFromFile(str.C_Str(), m_props->TexturesPath, typeName);
			m_Textures.emplace(texture->Name, texture); // add to loaded textures
		}
	}
	
	return texture;
}

void Fracture::AssetManager::ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene)
{
	
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{		
		aiMesh* mesh = scene->mMeshes[i];		
		aiString meshname = node->mName;
		FRACTURE_TRACE("loading Mesh: {}", meshname.C_Str());
		std::shared_ptr<Mesh> m_mesh = processMesh(model, mesh, scene, node);
		model->addMesh(m_mesh);	
		model->addMaterial(m_mesh->MaterialName);
	}

	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		FRACTURE_TRACE("loading Child: {}", node->mChildren[i]->mName.data);
		ProcessNode(model, node->mChildren[i], scene);
	}
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::TextureFromFile(const char* path, const std::string& directory, Fracture::TextureType texType, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '\\' + filename;
	std::shared_ptr<Fracture::Texture> texture = std::shared_ptr<Fracture::Texture>(new Texture(texType));

	switch (texType)
	{
		case TextureType::Diffuse:
		{
			texture->type = "material.diffuse";
			break;
		}
		case TextureType::Specular:
		{
			texture->type = "material.specular";
			break;
		}
		case TextureType::Normal:
		{
			texture->type = "material.normal";
			break;
		}
		case TextureType::Height:
		{
			texture->type = "material.height";
			break;
		}
		case TextureType::Bump:
		{
			texture->type = "material.bump";
			break;
		}
		case TextureType::Reflection:
		{
			texture->type = "material.reflection";
			break;
		}
		case TextureType::ColorAttachment:
		{
			texture->type = "ColorAttachment";
			break;
		}
		case TextureType::DepthStencilAttachment:
		{
			texture->type = "DepthStencilAttachment";
			break;
		}
		case TextureType::AO:
		{
			texture->type = "material.AO";
			break;
		}
		case TextureType::Metallic:
		{
			texture->type = "material.metallic";
			break;
		}
		case TextureType::Roughness:
		{
			texture->type = "material.roughness";
			break;
		}
	}
	
	texture->Name = path;
	texture->path = filename;
	texture->m_data = stbi_load(filename.c_str(), &texture->width, &texture->height, &texture->channel, 0);
	
	if (texture->m_data)
	{
		GLenum format;
		if (texture->channel == 1)
			format = GL_RED;
		else if (texture->channel == 3)
			format = GL_RGB;
		else if (texture->channel == 4)
			format = GL_RGBA;
		
		texture->Format = format;
		texture->Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->m_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT); // format == GL_RGBA ? GL_CLAMP_TO_EDGE : for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //format == GL_RGBA ? GL_CLAMP_TO_EDGE :
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texture->m_data);
		
	}
	else
	{
		FRACTURE_ERROR("Failed to load texture from file: {}",path);
		stbi_image_free(texture->m_data);
	}
	texture->Unbind();
	return texture;
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::loadTexture(std::string name, std::string path, Fracture::TextureType texType)
{
	std::shared_ptr<Texture> newTex = std::shared_ptr<Texture>(new Texture(name, texType));
	newTex->path = path;
	newTex->m_data = stbi_load(path.c_str(), &newTex->width, &newTex->height, &newTex->channel, 0);
	if (newTex->m_data)
	{
		GLenum format;
		if (newTex->channel == 1)
			format = GL_RED;
		else if (newTex->channel == 3)
			format = GL_RGB;
		else if (newTex->channel == 4)
			format = GL_RGBA;

		//GL_TEXTURE_2D;

		newTex->Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, newTex->width, newTex->height, 0, format, GL_UNSIGNED_BYTE, newTex->m_data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(newTex->m_data);
		newTex->Unbind();
	}
	else
	{
		FRACTURE_ERROR("Failed to load texture");
		stbi_image_free(newTex->m_data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return newTex;
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::HDRFromFile(std::string name, const char* path, Fracture::TextureType texType, bool gamma)
{
	std::shared_ptr<Fracture::Texture> texture = std::shared_ptr<Fracture::Texture>(new Texture(texType));
	texture->path = path;	
	texture->Name = name;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path, &texture->width, &texture->height, &texture->channel, 0);
	if (data)
	{
		GLenum format;
		if (texture->channel == 1)
			format = GL_RED;
		else if (texture->channel == 3)
			format = GL_RGB;
		else if (texture->channel == 4)
			format = GL_RGBA;
		texture->Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texture->width, texture->height, 0, format, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);

	}
	else
	{
		FRACTURE_ERROR("Failed to load texture {}", path);
		stbi_image_free(texture->m_data);
	}
	texture->Unbind();
	return texture;
}

