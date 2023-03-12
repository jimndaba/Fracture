#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANGER_H

#include "Scene.h"

namespace Fracture
{
	struct SceneRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
		//TODO Scene VERSIONING {probable best to use time stamp}
	};

	class SceneManager
	{


	public:
		SceneManager();

		static std::shared_ptr<Scene> CreateNewScene(const UUID& root = UUID());

		static UUID& AddEntity();
		static void AddEntity(const UUID& entity);
		static void RemoveEntity(const UUID& entity);

		template< class T, typename... Args >
		static void AddComponent(const UUID& entity, Args&&... params);

		template< class T, typename... Args >
		static void AddComponent(const UUID& entity, const std::shared_ptr<IComponent>& component);

		template<class T>
		static void RemoveComponent(const UUID& entity);

		template<class T>
		static void RemoveComponent(const std::shared_ptr<IComponent>& component);

		template <class T>
		static T* GetComponent(const UUID& entity);

		template <class T>
		static std::vector<T*>GetAllComponents();

		template<class T>
		static bool HasComponent(const UUID& id);


		static void AttachScript();
		static void DettachScript();

		static Scene* CurrentScene();

		static void SetScene(const std::shared_ptr<Scene>& scene);
		static void SetSceneByName(const std::string& scene);
		static void SetSceneByID(const UUID& scene);

		static void RegisterScene(const SceneRegistry& reg);

		static std::shared_ptr<Scene> LoadSceneFromFile(const std::string& path);
		static void SaveSceneToFile(const std::string& path);

	private:

		static std::shared_ptr<Scene> mCurrentScene;

	};



	template<class T, typename ...Args>
	inline void SceneManager::AddComponent(const UUID& entity, Args && ...params)
	{
		if (mCurrentScene)
		{
			auto component = std::make_shared<T>(entity, params...);
			mCurrentScene->ComponentReg[typeid(T)][entity] = component;
		}
	}

	template< class T, typename... Args >
	inline void SceneManager::AddComponent(const UUID& entity, const std::shared_ptr<IComponent>& component)
	{
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
	inline void SceneManager::RemoveComponent(const std::shared_ptr<IComponent>& component)
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
	inline T* SceneManager::GetComponent(const UUID& entity)
	{
		if (mCurrentScene)
		{
			if (mCurrentScene->ComponentReg[typeid(T)][entity])
			{
				return static_cast<T*>(mCurrentScene->ComponentReg[typeid(T)][entity].get());
			}
		}
		return nullptr;
	}

	template<class T>
	inline std::vector<T*> SceneManager::GetAllComponents()
	{
		std::vector<T*> components;
		if (mCurrentScene)
		{
			const auto& reg = mCurrentScene->ComponentReg[typeid(T)];
			for (const auto& component : reg)
			{
				if(component.second)
					components.push_back(static_cast<T*>(component.second.get()));
			}
		}
		return components;
	}

	template<class T>
	inline bool SceneManager::HasComponent(const UUID& id)
	{
		return GetComponent<T>(id);
	}

}

#endif