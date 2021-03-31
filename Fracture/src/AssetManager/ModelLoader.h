#pragma once
#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

namespace Fracture
{
	class StaticMesh;
	class SkeletonMesh;
	class Model;
	class Material;
	struct Bone;
	class Skeleton;	
	class Texture2D;
	class AnimationClip;
	enum class TextureType;

	class ModelLoader
	{
	public:
		ModelLoader();
		static std::shared_ptr<Model> LoadModel(const std::string& name, const std::string& path);
		static std::shared_ptr<StaticMesh> LoadStaticMesh(const std::string& name, const std::string& path);
		static std::shared_ptr<SkeletonMesh> LoadSkeletonMesh(const std::string& name, const std::string& path);
		static std::shared_ptr<StaticMesh> GeneratePlane(const int& width,const int& depth);


	private:

		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles			
			aiProcess_GenNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |            // Convert UVs if required 			
			aiProcess_ValidateDataStructure |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_SortByPType |
			aiProcess_OptimizeMeshes |
			aiProcess_PopulateArmatureData
			;

		//Functions
		static void ProcessNode(std::shared_ptr<Model> model, aiNode* node, const aiScene* scene);
		
		static std::shared_ptr<StaticMesh> ProcessStaticMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiNode* node);
		
		static std::shared_ptr<SkeletonMesh> ProcessSkeletonMesh(std::shared_ptr<Model> model, aiMesh* mesh, const aiScene* scene, aiNode* node);

		static void processSkeleton(std::shared_ptr<Bone> bone, std::shared_ptr<Skeleton> skeleton, aiNode* node);
		static void ImportMaterial(aiMaterial* material, std::shared_ptr<Material> f_materail);
		static std::shared_ptr<Texture2D> loadMaterialTexture(aiMaterial* mat, aiTextureType type, TextureType typeName);
		static std::shared_ptr<Fracture::Material> loadMeshMaterial(int index,aiMaterial* material, bool isAnimated);
		static std::shared_ptr<AnimationClip> loadModelAnimations(aiAnimation* animation);
	};
}

#endif