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
	struct AsyncLoadMeshEvent : public Event
	{
		AsyncLoadMeshEvent(UUID id) : Event(),MeshID(id) {};
		UUID MeshID;
		const char* Name() { return  "Async Load Mesh"; };
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
		static bool IsMeshLoaded(const std::string& Name);
		static bool IsMeshLoaded(const UUID& Name);
		static UUID GetMeshID(const std::string& Name);
		static StaticMesh* GetStaticMesh(const std::string& Name);
		static StaticMesh* GetStaticByIDMesh(const UUID& Name);
		static bool HasMeshPath(const std::string& path);

		static void RegisterShader(const ShaderRegistry& reg);		
		static Shader* GetShader(const std::string& Name);
		static Shader* GetShaderByID(const Fracture::UUID& id);

		void OnAsyncLoadMesh(const std::shared_ptr<AsyncLoadMeshEvent>& evnt);

		static std::map<UUID, MeshRegistry> mMeshRegister;
		static std::map<std::string, UUID> mMeshIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<StaticMesh>> mMeshes;


		static std::map<UUID, ShaderRegistry> mShaderRegister;
		static std::map<std::string, UUID> mShaderIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Shader>> mShaders;


		static AssetManager* Instance();

	private:
		static std::vector<UUID> mLoadedMeshes;
		static std::unordered_map<UUID,std::future<std::shared_ptr<Fracture::StaticMesh>>> mMeshFutures;
		static std::queue<MeshRegistry> mMeshesToLoad;

		static std::unique_ptr<AssetManager> mInstance;
	};





}




#endif // !ASSETMANAGER_H
