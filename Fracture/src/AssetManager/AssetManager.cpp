#include "AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Model.h"
#include "Rendering/Texture.h"
#include "Rendering/Vertex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

std::unique_ptr<Fracture::AssetManager> Fracture::AssetManager::m_instance;

std::map<std::string, std::shared_ptr<Fracture::Mesh>>  Fracture::AssetManager::m_meshes;
std::map<std::string, std::shared_ptr<Fracture::Texture>> Fracture::AssetManager::m_Textures;
std::map<std::string, std::shared_ptr<Fracture::Model>> Fracture::AssetManager::m_Models;
std::map<std::string, std::shared_ptr<Fracture::Shader>> Fracture::AssetManager::m_Shaders;
std::map<std::string, std::shared_ptr<Fracture::Material>> Fracture::AssetManager::m_Materials;

Fracture::AssetManager::AssetManager()
{
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
	m_shader->Name = name;
	m_shader->vertexPath = vertex;
	m_shader->fragPath = fragment;
	m_Shaders.emplace(name, m_shader);
}


void Fracture::AssetManager::AddModel(std::string name, std::string path)
{
	std::shared_ptr<Model> model = loadModel(path);
	model->Name = name;
	model->directory = path;
	m_Models.emplace(name, model);
}


void Fracture::AssetManager::AddTexture(std::string name, std::string path, TextureType mtype)
{
	std::shared_ptr<Texture> texture = loadTexture(name,path,mtype);
	m_Textures.emplace(name, texture);
}


void Fracture::AssetManager::AddMesh(std::string name, std::string path)
{
}

void Fracture::AssetManager::AddMaterial(std::string name, std::shared_ptr<Shader> shader)
{
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(name,shader));
	m_Materials.emplace(name, material);
}

void Fracture::AssetManager::AddMaterial(std::string name, std::shared_ptr<Material> material)
{
	m_Materials.emplace(name, material);
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

std::shared_ptr<Fracture::Model> Fracture::AssetManager::loadModel(std::string path)
{
	std::shared_ptr<Model> m_model = nullptr;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);//| aiProcess_CalcTangentSpace
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return 0;
	}

	m_model = std::shared_ptr<Model>(new Model());
	m_model->directory = path.substr(0, path.find_last_of('/'));
	// process ASSIMP's root node recursively
	ProcessNode(m_model, scene->mRootNode, scene);
	return m_model;
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::loadTexture(std::string name,std::string path,Fracture::TextureType texType)
{
	std::shared_ptr<Texture> newTex = std::shared_ptr<Texture>(new Texture(name,texType));
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glGenerateMipmap(GL_TEXTURE_2D);
	
		stbi_image_free(newTex->m_data);
		newTex->Unbind();
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(newTex->m_data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return newTex;
}

std::shared_ptr<Fracture::Mesh> Fracture::AssetManager::processMesh(std::shared_ptr<Model> model,aiMesh* mesh, const aiScene* scene)
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
			

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// process material
	// 1. diffuse maps
	
	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(model,material, aiTextureType_DIFFUSE, TextureType::Diffuse);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(model,material, aiTextureType_SPECULAR, TextureType::Specular);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(model, material, aiTextureType_HEIGHT, TextureType::Height);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(model,material, aiTextureType_AMBIENT, TextureType::Height);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	
	return std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures));
}

std::vector<std::shared_ptr<Fracture::Texture>> Fracture::AssetManager::loadMaterialTextures(std::shared_ptr<Fracture::Model> model,aiMaterial* mat, aiTextureType type, Fracture::TextureType typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_Textures.size(); j++)
		{
			for(const auto& component_pair :  m_Textures)
			{
				if (std::strcmp(component_pair.first.c_str(), str.C_Str()) == 0)
				{
					textures.push_back(component_pair.second);
					skip = true;
					break;
				}
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			std::shared_ptr<Texture> texture = TextureFromFile(str.C_Str(), model->directory,typeName);
			texture->textureType = typeName;
			texture->path = str.C_Str();
			textures.push_back(texture);
			m_Textures.emplace(texture->Name,texture); // add to loaded textures
		}
	}
	return textures;
}

void Fracture::AssetManager::ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->addMesh(processMesh(model,mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(model, node->mChildren[i], scene);
	}


}

void Fracture::AssetManager::ProcessNode(aiNode* node, const aiScene* scene)
{
	//after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Fracture::Texture> Fracture::AssetManager::TextureFromFile(const char* path, const std::string& directory, Fracture::TextureType texType, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::shared_ptr<Fracture::Texture> texture = std::shared_ptr<Fracture::Texture>(new Texture(texType));
	
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

		texture->Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->m_data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texture->m_data);
		
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(texture->m_data);
	}
	texture->Unbind();
	return texture;
}

