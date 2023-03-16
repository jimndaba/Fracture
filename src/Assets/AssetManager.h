#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <thread>
#include <future>
#include <queue>
#include "AssetRegistries.h"
#include "EventSystem/Eventbus.h"

namespace Fracture
{
	struct StaticMesh;
	struct Shader;	
	struct Scene;

	struct Texture;
	struct Material;

	struct AsyncLoadMeshEvent : public Event
	{
		AsyncLoadMeshEvent(UUID id) : Event(),MeshID(id) {};
		UUID MeshID;
		const char* Name() { return  "Async Load Mesh"; };
	};

	struct AsyncLoadTextureEvent : public Event
	{
		AsyncLoadTextureEvent(UUID id) : Event(), TextureID(id) {};
		UUID TextureID;
		const char* Name() { return  "Async Load Texture"; };
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


		static bool IsMeshLoaded(const std::string& Name);
		static bool IsMeshLoaded(const UUID& Name);
		static bool IsTextureLoaded(const UUID& Name);
		static bool IsTextureLoaded(const std::string& Name);
		static UUID GetMeshID(const std::string& Name);
		static StaticMesh* GetStaticMesh(const std::string& Name);
		static StaticMesh* GetStaticByIDMesh(const UUID& Name);
		static bool HasMeshPath(const std::string& path);
		static bool HasTexturePath(const std::string& path);

		static void RegisterShader(const ShaderRegistry& reg);		
		static Shader* GetShader(const std::string& Name);
		static Shader* GetShaderByID(const Fracture::UUID& id);

		static void RegisterTexture(const TextureRegistry& reg);
		static Texture* GetTexture(const std::string& Name);
		static Texture* GetTextureByID(const Fracture::UUID& id);

		void OnAsyncLoadMesh(const std::shared_ptr<AsyncLoadMeshEvent>& evnt);
		void OnAsyncLoadTexture(const std::shared_ptr<AsyncLoadTextureEvent>& evnt);

		static std::map<UUID, MeshRegistry> mMeshRegister;
		static std::map<std::string, UUID> mMeshIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<StaticMesh>> mMeshes;


		static std::map<UUID, ShaderRegistry> mShaderRegister;
		static std::map<std::string, UUID> mShaderIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Shader>> mShaders;

		static std::map<UUID, TextureRegistry> mTextureRegister;
		static std::map<std::string, UUID> mTextureIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Texture>> mTextures;


		static AssetManager* Instance();

	private:
		static std::vector<UUID> mLoadedMeshes;
		static std::unordered_map<UUID,std::future<std::shared_ptr<Fracture::StaticMesh>>> mMeshFutures;
		static std::queue<MeshRegistry> mMeshesToLoad;

		static std::vector<UUID> mLoadedTextures;
		static std::unordered_map<UUID, std::future<std::shared_ptr<Fracture::Texture>>> mTextureFutures;
		static std::queue<TextureRegistry> mTexturesToLoad;

		static std::unique_ptr<AssetManager> mInstance;
	};





}




#endif // !ASSETMANAGER_H
