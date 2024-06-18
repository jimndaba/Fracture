#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANGER_H

#include "Scene.h"
#include "Assets/AssetRegistries.h"


namespace Fracture
{
	class LuaScript;
	struct DestroyEntityEvent;
	class PrefabFactory;

	class SceneManager
	{
	public:
		SceneManager();

		static std::shared_ptr<Scene> CreateNewScene(const UUID& root = UUID());

		static UUID AddEntity();
		static void AddEntity(const UUID& entity);
		static void RemoveEntity(const UUID& entity);
		static void RemovePrefab(const UUID& entity);

		template< class T, typename... Args >
		static void AddComponent(const UUID& entity, Args&&... params);

		template< class T, typename... Args >
		static void AddComponentByInstance(const UUID& entity, const std::shared_ptr<T>& component);


		template<class T>
		static void RemoveComponent(const UUID& entity);

		template<class T>
		static void RemoveComponent(const std::shared_ptr<T>& component);

		template <class T>
		static std::shared_ptr<T> GetComponent(const UUID& entity);

		template<class T>
		static std::shared_ptr<T> GetInstanceComponent(UUID scene, const UUID& entity);

		template <class T>
		static std::vector<std::shared_ptr<T>>GetAllComponents();

		template<class T>
		static bool HasComponent(const UUID& id);

		static bool IsTaggedWith(const UUID& id,const std::string& tag);
		static UUID GetEntityWithTag(const std::string& tag);

		static bool IsPrefabScene(const UUID& id);
		static ScenePrefab GetScenePrefab(const UUID& id);

		static void AttachScript(const UUID& entity_id);
		static void AttachScript(const UUID& entity_id,const UUID& script_id);
		static void DettachScript(const UUID& entity_id, const UUID& script_id);
		static std::shared_ptr<ScriptComponent> GetScript(const UUID& entity_id, const UUID& script_id);
		static std::vector<std::shared_ptr<ScriptComponent>> GetAllEntityScripts(const UUID& entity_id);
		static bool HasScripts(const UUID& entity_id);

		static Scene* CurrentScene();
		static std::shared_ptr<CameraComponent> ActiveCamera();
		static void SetActiveCamera(const std::shared_ptr<CameraComponent>& mcamera);
		static void SetActiveCamera(const Fracture::UUID& camera_id);

		static void LoadScene(const std::string& scene);
		static UUID& LoadSceneFromFile(const std::string& path);
		static void InstanceSceneFromFile(ScenePrefab prefab);
		static std::shared_ptr<Scene> DirectLoadScene(const std::string& path);


		static std::map<UUID,int> LoadSceneByID(const UUID& scene);
		static void UnloadScene(const std::string& scene);
		static void UnloadSceneByID(const UUID& scene);

		static void SetScene(const std::shared_ptr<Scene>& scene);
		static void SetSceneByName(const std::string& scene);
		static std::map<UUID, int> SetSceneByID(const UUID& scene);

		static void RegisterScene(const SceneRegistry& reg);
		static bool HasScenePath(const std::string& path);

		static std::map<UUID, SceneRegistry> mSceneRegister;
		static std::map<std::string, UUID> mSceneIDLookUp;
		static std::unordered_map<UUID, std::shared_ptr<Scene>> mScenes;	
		static std::unordered_map<UUID, std::vector<UUID>> mScript_Entities;

		static UUID Instantiate(UUID prefab, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::quat rotation = glm::quat());
		static UUID InstantiateAsChildOf(UUID prefab,UUID parent, glm::vec3 position);
		static UUID Instantiate(ScenePrefab prefab);

		template<class T, typename... Args>
		static void InstanceComponent(UUID prefab, UUID new_entity, UUID original_entity, Args&&... params);

		static void  InstanceComponents(UUID prefab, UUID new_entity, UUID original_entity, UUID parent_entity);

		static void Destroy(UUID entity);

		template<class T>
		static void RemoveComponentIfExists(Fracture::UUID entity);

		static void AddComponentInstance(std::shared_ptr<TagComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<HierachyComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<TransformComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<MeshComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<PointlightComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<SpotlightComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<SunlightComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<ColliderComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<RigidbodyComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<AudioSourceComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<ScriptComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<CameraComponent>& component, UUID new_entity);
		static void AddComponentInstance(std::shared_ptr<AnimationComponent>& component, UUID new_entity);


		void OnDestroyEntity(const std::shared_ptr<DestroyEntityEvent>& evnt);

		static std::unique_ptr<PrefabFactory> mPrefabFactor;

	private:

		static std::shared_ptr<Scene> mCurrentScene;
		static std::shared_ptr<CameraComponent> mActiveCamera;

	};



	template<class T, typename ...Args>
	inline void SceneManager::AddComponent(const UUID& entity, Args && ...params)
	{
		if (!mCurrentScene)
			return;

		if (mCurrentScene)
		{
			auto component = std::make_shared<T>(entity, params...);
			mCurrentScene->ComponentReg[typeid(T)][entity] = component;
		}
	}

	template< class T, typename... Args >
	inline void SceneManager::AddComponentByInstance(const UUID& entity, const std::shared_ptr<T>& component)
	{
		if (!mCurrentScene)
			return;

		if (mCurrentScene)
		{		
			mCurrentScene->ComponentReg[typeid(T)][entity] = std::move(component);
		}
	}

	template<class T>
	inline void SceneManager::RemoveComponent(const UUID& entity)
	{

		if (mCurrentScene)
		{
			if (!mCurrentScene->ComponentReg[typeid(T)].empty())
			{	
				if (mCurrentScene->ComponentReg[typeid(T)][entity])
				{
					mCurrentScene->ComponentReg[typeid(T)].erase(entity);
				}				
			}
		}
	}

	template<class T>
	inline void SceneManager::RemoveComponent(const std::shared_ptr<T>& component)
	{
		if (mCurrentScene)
		{
			if (!mCurrentScene->ComponentReg[typeid(T)].empty())
			{
				RemoveComponent(component->GetID());
			}
		}
	}

	template<class T>
	inline std::shared_ptr<T> SceneManager::GetComponent(const UUID& entity)
	{
		if (mCurrentScene)
		{
			if (mCurrentScene->ComponentReg[typeid(T)][entity])
			{
				return std::dynamic_pointer_cast<T>(mCurrentScene->ComponentReg[typeid(T)][entity]);
			}			
		}
		return nullptr;
	}

	template<class T>
	inline std::shared_ptr<T> SceneManager::GetInstanceComponent(UUID scene, const UUID& entity)
	{
		if (mScenes[scene])
		{
			if (mScenes[scene]->ComponentReg[typeid(T)][entity])
			{
				return std::dynamic_pointer_cast<T>(mScenes[scene]->ComponentReg[typeid(T)][entity]);
			}
		}
		return nullptr;
	}

	template<class T>
	inline std::vector<std::shared_ptr<T>> SceneManager::GetAllComponents()
	{
		std::vector<std::shared_ptr<T>> components;
		if (mCurrentScene)
		{
			const auto& reg = mCurrentScene->ComponentReg[typeid(T)];
			for (const auto& component : reg)
			{
				if(component.second)
					components.push_back(std::dynamic_pointer_cast<T>(component.second));
			}
		}
		return components;
	}

	template<class T>
	inline bool SceneManager::HasComponent(const UUID& id)
	{
		if ((mCurrentScene->ComponentReg[typeid(T)].find(id) != mCurrentScene->ComponentReg[typeid(T)].end()))
				return true;


		if (mCurrentScene->mScriptReg.find(id) != mCurrentScene->mScriptReg.end())
		{
			for (const auto& script : mCurrentScene->mScriptReg[id])
			{
				if (script->GetID() == id && dynamic_cast<T*>(script.get()))
					return true;
			}
		}

		return false;
	}

	template<class T, typename ...Args>
	inline void SceneManager::InstanceComponent(UUID prefab, UUID new_entity, UUID original_entity, Args && ...params)
	{
		if (mScenes[prefab]->ComponentReg[typeid(T)][original_entity])
		{
			auto component = std::dynamic_pointer_cast<T>(mScenes[prefab]->ComponentReg[typeid(T)][original_entity]);
			AddComponentInstance(component,new_entity);
			//AddComponentByInstance<T>(new_entity, std::make_shared<T>(*component.get(), new_entity));		
		}
	}

	template<class T>
	inline void SceneManager::RemoveComponentIfExists(Fracture::UUID entity)
	{
		if (HasComponent<T>(entity))
		{
			RemoveComponent<T>(entity);
		}
	}

	

}

#endif