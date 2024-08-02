#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <thread>
#include <future>
#include <queue>
#include "AssetRegistries.h"
#include "EventSystem/Eventbus.h"
#include "taskflow/taskflow.hpp"
#include "World/WorldEvents.h"

namespace Fracture
{
	struct StaticMesh;
	struct Shader;	
	struct Scene;

	struct Texture;
	struct Terrain;
	struct Material;
	struct AnimationClip;
	struct AnimationGraph;
	struct TextureCreationInfo;

	struct AsyncLoadMeshEvent : public Event
	{
		AsyncLoadMeshEvent(UUID id) : Event(),MeshID(id) {};
		UUID MeshID;
		const char* Name() { return  "Async Load Mesh"; };
	};

	struct AsyncLoadMeshAndAttachEvent : public Event
	{
		AsyncLoadMeshAndAttachEvent(UUID entity,UUID mesh) : Event(), MeshID(mesh),EntityID(entity) {};
		UUID MeshID;
		UUID EntityID;
		const char* Name() { return  "Async Load Mesh"; };
	};

	struct AsyncLoadTextureEvent : public Event
	{
		AsyncLoadTextureEvent(UUID id) : Event(), TextureID(id) {};
		UUID TextureID;
		const char* Name() { return  "Async Load Texture"; };
	};

	struct AsyncLoadMaterialEvent : public Event
	{
		AsyncLoadMaterialEvent(UUID id) : Event(), MaterialID(id) {};
		UUID MaterialID;
		const char* Name() { return  "Async Load Material"; };
	};

	struct LoadandAttachContext
	{
		UUID Mesh;
		UUID Entity;

		// `operator==` is required to compare keys in case of a hash collision
		bool operator==(const LoadandAttachContext& p) const {
			return Mesh == p.Mesh && Entity == p.Entity;
		}
	};

	class AssetManager
	{
	public:
		AssetManager();

		void RegisterCallbacks(Eventbus* bus);

		static void OnInit(const std::string& assetfilepath);
		static void OnSave(const std::string& path);
		static void OnLoad();

		static void RegisterMesh(const MeshRegistry& reg);
		static void LoadMesh(const std::string& Name);


		static void AsyncLoadMesh(const std::string& Name);
		static void AsyncLoadMeshByID(const UUID& id);
		static void AsyncLoadTexture(const std::string& Name);
		static void AsyncLoadTextureByID(const UUID& id);
		static void AsyncLoadMaterial(const std::string& Name);
		static void AsyncLoadMaterialByID(const UUID& id);
		static void AsyncLoadAnimation(const std::string& Name);
		static void AsyncLoadAnimationByID(const UUID& id);

		static void AddTexture(TextureCreationInfo& desc);

		static bool IsMeshLoaded(const std::string& Name);
		static bool IsMeshLoaded(const UUID& Name);

		static bool IsTextureLoaded(const UUID& Name);
		static bool IsTextureLoaded(const std::string& Name);

		static bool IsMaterialLoaded(const UUID& Name);
		static bool IsMaterialLoaded(const std::string& Name);

		static bool IsAnimationLoaded(const UUID& Name);
		static bool IsAnimationLoaded(const std::string& Name);

		static bool IsTerrainLoaded(const UUID& Name);
		static bool IsTerrainLoaded(const std::string& Name);

		static UUID GetMeshID(const std::string& Name);
		static std::shared_ptr<Fracture::StaticMesh> GetStaticMesh(const std::string& Name);
		static std::shared_ptr<Fracture::StaticMesh> GetStaticByIDMesh(const UUID& Name);

		static bool HasMeshPath(const std::string& path);
		static bool HasTexturePath(const std::string& path);
		static bool HasMaterialPath(const std::string& path);
		static bool HasTerrainPath(const std::string& path);

		static void RegisterShader(const ShaderRegistry& reg);		
		static std::shared_ptr<Shader> GetShader(const std::string& Name);
		static std::shared_ptr<Shader> GetShaderByID(const Fracture::UUID& id);

		static void RegisterTexture(const TextureRegistry& reg);
		static std::shared_ptr<Texture> GetTexture(const std::string& Name);
		static std::shared_ptr<Texture> GetTextureByID(const Fracture::UUID& id);

		static void RegisterMaterial(const MaterialRegistry& reg);
		static std::shared_ptr<Material> GetMaterial(const std::string& Name);
		static std::shared_ptr<Material> GetMaterialByID(const Fracture::UUID& id);

		static void RegisterAnimation(const AnimationClipRegistry& reg);
		static std::shared_ptr<AnimationClip> GetAnimation(const std::string& Name);
		static std::shared_ptr<AnimationClip> GetAnimationByID(const Fracture::UUID& id);

		static void RegisterAnimationGraph(const AnimationGraphRegistry& reg);
		static std::shared_ptr<AnimationGraph> GetAnimationGraph(const std::string& Name);
		static std::shared_ptr<AnimationGraph> GetAnimationGraphByID(const Fracture::UUID& id);

		static void RegisterTerrain(const TerrainRegistry& reg);
		static std::shared_ptr<Terrain> GetTerrain(const std::string& Name);
		static std::shared_ptr<Terrain> GetTerrainByID(const Fracture::UUID& id);


		void OnAsyncLoadMesh(const std::shared_ptr<AsyncLoadMeshEvent>& evnt);
		void OnAsyncLoadTexture(const std::shared_ptr<AsyncLoadTextureEvent>& evnt);
		void OnAsyncLoadMaterial(const std::shared_ptr<AsyncLoadMaterialEvent>& evnt);
		void OnAsyncLoadandAttach(const std::shared_ptr<AsyncLoadMeshAndAttachEvent>& evnt);
		void OnLoadTexture(const std::shared_ptr<LoadTextureEvent>& evnt);

		static std::map<UUID, MeshRegistry> mMeshRegister;
		static std::map<std::string, UUID> mMeshIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<StaticMesh>> mMeshes;


		static std::map<UUID, ShaderRegistry> mShaderRegister;
		static std::map<std::string, UUID> mShaderIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Shader>> mShaders;

		static std::map<UUID, TextureRegistry> mTextureRegister;
		static std::map<std::string, UUID> mTextureIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Texture>> mTextures;

		static std::map<UUID, TerrainRegistry> mTerrainRegister;
		static std::map<std::string, UUID> mTerrainIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Terrain>> mTerrains;
		static std::vector<UUID> mLoadedTerrains;

		static std::map<UUID, MaterialRegistry> mMaterialRegister;
		static std::map<std::string, UUID> mMaterialIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Material>> mMaterials;

		static std::map<UUID, AnimationClipRegistry> mAnimationRegister;
		static std::map<std::string, UUID> mAnimationIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<AnimationClip>> mAnimations;

		static std::map<Fracture::UUID, Fracture::AnimationGraphRegistry> mAnimationGraphRegister;
		static std::map<std::string, Fracture::UUID> mAnimationGraphIDLookUp;

		static int CountUseCountMesh(UUID mesh);


		static AssetManager* Instance();

		static bool IsRegisterDirty;

	private:
		static std::vector<UUID> mLoadedMeshes;
		static std::unordered_map<UUID,std::future<std::shared_ptr<Fracture::StaticMesh>>> mMeshFutures;
		static std::unordered_map<LoadandAttachContext,std::future<std::shared_ptr<Fracture::StaticMesh>>> mMeshAndAttachFutures;
		static std::queue<MeshRegistry> mMeshesToLoad;
		static std::queue<LoadandAttachContext> mMeshesToLoadandAttach;

		static std::vector<UUID> mLoadedTextures;
		static std::unordered_map<UUID, std::future<std::shared_ptr<Fracture::Texture>>> mTextureFutures;
		static std::queue<TextureRegistry> mTexturesToLoad;

		static std::vector<UUID> mLoadedMaterials;
		static std::unordered_map<UUID, std::future<std::shared_ptr<Fracture::Material>>> mMaterialFutures;
		static std::queue<MaterialRegistry> mMaterialToLoad;

		static std::unordered_map<UUID, std::future<std::shared_ptr<Fracture::Terrain>>> mTerrainFutures;
		static std::queue<TerrainRegistry> mTerrainToLoad;


		static std::vector<UUID> mLoadedAnimations;
		static std::unordered_map<UUID, std::future<std::shared_ptr<Fracture::AnimationClip>>> mAnimationFutures;
		static std::queue<AnimationClipRegistry> mAnimationsToLoad;

		static tf::Taskflow taskflow;

		static std::unique_ptr<AssetManager> mInstance;
	};

}

namespace std
{
	template<>
	struct hash<Fracture::LoadandAttachContext>
	{
		std::size_t operator() (const Fracture::LoadandAttachContext& node) const
		{
			std::size_t h1 = std::hash<uint32_t>()(node.Entity);
			std::size_t h2 = std::hash<uint32_t>()(node.Mesh);

			return h1 ^ h2;
		}
	};


}



#endif // !ASSETMANAGER_H
