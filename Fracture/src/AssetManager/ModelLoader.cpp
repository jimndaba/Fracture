#include "ModelLoader.h"
#include "AssetManager.h"
#include "TextureLoader.h"
#include "Serialisation/ProjectProperties.h"
#include "Rendering/OpenGL/Vertex.h"
#include "Rendering/OpenGL/Mesh.h"
#include "Rendering/Model.h"
#include "Animation/Skeleton.h"
#include "Rendering/OpenGL/StaticMesh.h"
#include "Rendering/OpenGL/SkeletonMesh.h"
#include "Rendering/Material.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Animation/AnimationClip.h"
#include "Math/Math.h"
#include "Logging/Logger.h"

Fracture::ModelLoader::ModelLoader()
{
}

std::shared_ptr<Fracture::Model> Fracture::ModelLoader::LoadModel(const std::string& name, const std::string& path)
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

	if (m_model->m_IsAnimated)
	{
		processSkeleton(m_model->m_Skeleton->m_Root, m_model->m_Skeleton, scene->mRootNode);
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		m_model->m_materials.resize(scene->mNumMaterials);

		m_model->m_materials[i] = loadMeshMaterial(scene->mMaterials[i], scene->HasAnimations());
	}

	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		m_model->m_animations.resize(scene->mNumAnimations);
		m_model->m_animations[i] = loadModelAnimations(scene->mAnimations[i]);
	}

	FRACTURE_ERROR("Number of Animations: {}", m_model->m_animations.size());

	return m_model;
}

std::shared_ptr<Fracture::StaticMesh> Fracture::ModelLoader::LoadStaticMesh(const std::string& name, const std::string& path)
{
	
}

std::shared_ptr<Fracture::SkeletonMesh> Fracture::ModelLoader::LoadSkeletonMesh(const std::string& name, const std::string& path)
{
	return std::shared_ptr<SkeletonMesh>();
}

void Fracture::ModelLoader::ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene)
{
	bool isAnimated = scene->HasAnimations();

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		aiString meshname = node->mName;
		std::shared_ptr<Mesh> m_mesh;

		FRACTURE_TRACE("loading Mesh: {}", meshname.C_Str());

		if (isAnimated)
		{
			m_mesh = ProcessSkeletonMesh(model, mesh, scene, node);
		}
		else
		{
			m_mesh = ProcessStaticMesh(model, mesh, scene, node);
		}
		
		model->addMesh(m_mesh);
	}

	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		FRACTURE_TRACE("loading Child: {}", node->mChildren[i]->mName.data);
		ProcessNode(model, node->mChildren[i], scene);
	}
}

std::shared_ptr<Fracture::StaticMesh> Fracture::ModelLoader::ProcessStaticMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiNode* node)
{
	std::shared_ptr<StaticMesh> new_mesh;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture2D>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// process vertex positions, normals and texture coordinates
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.Uvs = vec;
		}
		else
		{
			vertex.Uvs = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			//Tangents
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
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

	new_mesh = StaticMesh::Create(vertices, indices, textures);

	aiMatrix4x4 transform = node->mTransformation;

	new_mesh->SetName(mesh_name);
	//new_mesh->ModelName = model->Name;
	new_mesh->SetMaterialIndex(mesh->mMaterialIndex);

	std::shared_ptr<BoundingBox> aabb = std::make_shared<BoundingBox>();
	//process aabb
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		aabb->min.x = glm::min(vertex.Position.x, aabb->min.x);
		aabb->min.y = glm::min(vertex.Position.y, aabb->min.y);
		aabb->min.z = glm::min(vertex.Position.z, aabb->min.z);
		aabb->max.x = glm::max(vertex.Position.x, aabb->max.x);
		aabb->max.y = glm::max(vertex.Position.y, aabb->max.y);
		aabb->max.z = glm::max(vertex.Position.z, aabb->max.z);
	}

	new_mesh->SetAABB(aabb);

	return new_mesh;
}


std::shared_ptr<Fracture::SkeletonMesh> Fracture::ModelLoader::ProcessSkeletonMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiNode* node)
{
	std::shared_ptr<SkeletonMesh> new_mesh;
	std::vector<AnimatedVertex> m_animatedvertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture2D>> textures;

	std::map<std::string, uint32_t> m_bonemap;
	std::vector<BoneInfo> m_boneInfo;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		AnimatedVertex vertex;
		glm::vec3 vector;
		// process vertex positions, normals and texture coordinates
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.Uvs = vec;
		}
		else
		{
			vertex.Uvs = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			//Tangents
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		m_animatedvertices.push_back(vertex);
	}

	auto m_skeleton = std::make_shared<Skeleton>();
	m_skeleton->BoneParents.resize(mesh->mNumBones);
	m_boneInfo.resize(mesh->mNumBones);

	uint32_t  m_NumBones = 0;
	int BoneIndex = 0;
	glm::mat4 globalInverse = Math::Mat4FromAssimpMat4(scene->mRootNode->mTransformation);
	m_skeleton->m_InverseTransform = glm::inverse(globalInverse);

	FRACTURE_INFO("No. Bones: {}", mesh->mNumBones);

	//populate bone map and info
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {

		std::string boneName = mesh->mBones[i]->mName.C_Str();

		if (m_bonemap.find(boneName) == m_bonemap.end()) {
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_boneInfo[i] = bi;
		}
		else {
			BoneIndex = m_bonemap[boneName];
		}

		m_bonemap[boneName] = BoneIndex;
		m_boneInfo[BoneIndex].ID = BoneIndex;
		m_boneInfo[BoneIndex].Name = boneName;
		m_boneInfo[BoneIndex].BoneOffset = Math::Mat4FromAssimpMat4(mesh->mBones[i]->mOffsetMatrix);


		assert(BoneIndex != -1);
		if (m_bonemap.find(mesh->mBones[i]->mNode->mParent->mName.data) != m_bonemap.end())
		{
			if (mesh->mBones[i]->mNode->mParent != nullptr)
			{
				m_skeleton->BoneParents[i] = m_bonemap[mesh->mBones[i]->mNode->mParent->mName.data];
			}
		}
		else
		{
			m_skeleton->BoneParents[i] = 0;
		}


		FRACTURE_INFO("BONE: {}", m_boneInfo[BoneIndex].Name);
		FRACTURE_INFO("BONE PARENT : {}", m_skeleton->BoneParents[BoneIndex]);
		for (size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = mesh->mBones[i]->mWeights[j].mWeight;
			m_animatedvertices[VertexID].AddBoneData(BoneIndex, Weight);
		}


	}

	m_skeleton->NumBones = mesh->mNumBones;
	m_skeleton->m_BoneMapping = m_bonemap;
	m_skeleton->m_BoneInfo = m_boneInfo;

	model->m_Skeleton = m_skeleton;
	FRACTURE_INFO("Root Name: {}", m_skeleton->m_Root->Name);



	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	aiMaterial* currentMaterial(scene->mMaterials[mesh->mMaterialIndex]);
	std::string mesh_name = node->mName.data;

	
	new_mesh = SkeletonMesh::Create(m_animatedvertices, indices, textures);
	
	aiMatrix4x4 transform = node->mTransformation;

	new_mesh->SetName(mesh_name);
	//new_mesh->ModelName = model->Name;
	new_mesh->SetMaterialIndex(mesh->mMaterialIndex);

	std::shared_ptr<BoundingBox> aabb = std::make_shared<BoundingBox>();
	//process aabb
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		aabb->min.x = glm::min(vertex.Position.x, aabb->min.x);
		aabb->min.y = glm::min(vertex.Position.y, aabb->min.y);
		aabb->min.z = glm::min(vertex.Position.z, aabb->min.z);
		aabb->max.x = glm::max(vertex.Position.x, aabb->max.x);
		aabb->max.y = glm::max(vertex.Position.y, aabb->max.y);
		aabb->max.z = glm::max(vertex.Position.z, aabb->max.z);
	}

	new_mesh->SetAABB(aabb);

	return new_mesh;
}

void Fracture::ModelLoader::processSkeleton(std::shared_ptr<Bone> bone, std::shared_ptr<Skeleton> skeleton, aiNode* node)
{
	std::shared_ptr<Bone> m_bone = std::make_shared<Bone>();
	m_bone->ID = skeleton->m_BoneMapping[node->mName.data];
	m_bone->Name = node->mName.data;
	m_bone->LocalTransformation = Math::Mat4FromAssimpMat4(node->mTransformation);
	bone->AddChild(m_bone);
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processSkeleton(m_bone, skeleton, node->mChildren[i]);
	}
}

void Fracture::ModelLoader::ImportMaterial(aiMaterial* material, std::shared_ptr<Material> f_materail)
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
		auto texture = loadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, TextureType::Diffuse);

		if (texture)
		{
			f_materail->setFloat("albedoFlag", 1.0f);
			f_materail->SetTexture("albedoMap", texture, (unsigned int)TextureType::Diffuse);
			//if (texture-> == TextureType::)
			//{
			//	f_materail->setIsTransparent(true);
			//	f_materail->setFloat("TransparencyFlag", 1.0f);
			//}
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
		std::shared_ptr<Texture2D> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_NORMALS, TextureType::Normal);
		if (texture)
		{
			f_materail->SetTexture("normalMap", texture,(unsigned int)TextureType::Normal);
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
		std::shared_ptr<Texture2D> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_SHININESS, TextureType::Roughness);
		if (texture)
		{
			f_materail->setFloat("roughnessFlag", 1.0f);
			f_materail->SetTexture("roughnessMap", texture, (unsigned int)TextureType::Roughness);
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
		std::shared_ptr<Texture2D> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_METALNESS, TextureType::Metallic);
		if (texture)
		{
			f_materail->setFloat("metallicFlag", 1.0f);
			f_materail->SetTexture("metallicMap", texture, (unsigned int)TextureType::Metallic);
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
		std::shared_ptr<Texture2D> texture = loadMaterialTexture(material, aiTextureType::aiTextureType_AMBIENT_OCCLUSION, TextureType::AO);
		if (texture)
		{
			f_materail->setFloat("aoFlag", 1.0f);
			f_materail->SetTexture("aoMap", texture, (int)TextureType::AO);
			//mi->Set("u_AlbedoTexToggle", 1.0f); - way to toggle texture on or off
		}
		else
		{
			FRACTURE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
			f_materail->setFloat("aoFlag", 0.0f);
			f_materail->setFloat("u_ao", 0.1f);
			// Fallback to albedo color				
		}
	}
	else
	{
		f_materail->setFloat("aoFlag", 0.0f);
		f_materail->setFloat("u_ao", 0.1f);
	}
}

std::shared_ptr<Fracture::Texture2D> Fracture::ModelLoader::loadMaterialTexture(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
	std::shared_ptr<Texture2D> texture;

	aiString str;
	mat->GetTexture(type, 0, &str);
	bool skip = false;

	auto m_Textures = AssetManager::GetTextures();
	for (unsigned int j = 0; j < m_Textures.size(); j++)
	{
		for (const auto& component_pair : m_Textures)
		{
			if (std::strcmp(component_pair.first.c_str(), str.C_Str()) == 0)
			{
				skip = true;
				auto texture = AssetManager::getTexture2D(str.C_Str());
				return texture;
			}
		}		
	}
	if (!skip)
	{   // if texture hasn't been loaded already, load it		
		texture = TextureLoader::LoadTexture2D(str.C_Str(), str.C_Str(), AssetManager::GetProperties()->TexturesPath);
		AssetManager::AddTexture2D(str.C_Str(), texture);
	}

	return texture;
}

std::shared_ptr<Fracture::Material> Fracture::ModelLoader::loadMeshMaterial(aiMaterial* material, bool isAnimated)
{
	std::string name = material->GetName().data;
	std::shared_ptr<Material> m_material;
	if (isAnimated)
	{
		m_material = std::make_shared<Material>(name, AssetManager::getShader("PBRAnimated")); //m_base->Create(name);
	}
	else
	{
		m_material = std::make_shared<Material>(name, AssetManager::getShader("PBRStatic")); //m_base->Create(name);
	}

	ImportMaterial(material, m_material);
	return m_material;
}

std::shared_ptr<Fracture::AnimationClip > Fracture::ModelLoader::loadModelAnimations(aiAnimation* animation)
{
	std::shared_ptr<AnimationClip> clip = std::make_shared<AnimationClip>();

	clip->FramesPerSec = (float)animation->mTicksPerSecond;
	clip->NumberOfFrames = (int)animation->mDuration;
	clip->Name = animation->mName.data;

	clip->m_channels.resize(animation->mNumChannels);

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		clip->m_channels[i].Name = animation->mChannels[i]->mNodeName.data;
		if (animation->mChannels[i]->mNumPositionKeys > 0)
		{
			for (unsigned int J = 0; J < animation->mChannels[i]->mNumPositionKeys; J++)
			{
				AnimationKeyframe keyframe;
				aiVector3D position = animation->mChannels[i]->mPositionKeys[J].mValue;
				keyframe.Position_key = glm::vec3{ position.x,position.y,position.z };
				keyframe.Time = animation->mChannels[i]->mPositionKeys[J].mTime;
				clip->m_channels[i].m_PositionKeys.push_back(keyframe);
			}
		}
		if (animation->mChannels[i]->mNumRotationKeys > 0)
		{
			for (unsigned int J = 0; J < animation->mChannels[i]->mNumRotationKeys; J++)
			{

				AnimationKeyframe keyframe;
				aiQuaternion rotation = animation->mChannels[i]->mRotationKeys[J].mValue;
				keyframe.Rotation_key = glm::quat{ rotation.x,rotation.y,rotation.z,rotation.w };
				keyframe.Time = animation->mChannels[i]->mRotationKeys[J].mTime;
				clip->m_channels[i].m_RotationKeys.push_back(keyframe);
			}

		}
		if (animation->mChannels[i]->mNumScalingKeys > 0)
		{
			for (unsigned int J = 0; J < animation->mChannels[i]->mNumScalingKeys; J++)
			{
				AnimationKeyframe keyframe;
				aiVector3D scale = animation->mChannels[i]->mScalingKeys[J].mValue;
				keyframe.Scale_key = glm::vec3{ scale.x,scale.y,scale.z };
				keyframe.Time = animation->mChannels[i]->mScalingKeys[J].mTime;
				clip->m_channels[i].m_ScaleKeys.push_back(keyframe);
			}
		}
	}

	FRACTURE_ERROR("Name of Animation: {}", animation->mName.data);
	FRACTURE_ERROR("Duration: {}", animation->mDuration);
	FRACTURE_ERROR("FPS: {}", animation->mTicksPerSecond);
	FRACTURE_ERROR("Number of Animations Channels: {}", clip->m_channels.size());

	return clip;
}
