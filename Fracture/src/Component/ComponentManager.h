#pragma once
#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H


#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <map>
#include "Logging/Logger.h"
#include "Profiling/Profiler.h"
#include "ComponentSet.h"

namespace Fracture
{
	class TagComponent;
	class RelationShipComponent;
	class TransformComponent;
	class CameraControllerComponent;
	class RigidBodyComponent;
	class RenderComponent;
	class ScriptComponent;
	class LightComponent;
	class BoxColliderComponent;
	class BillboardComponent;
	class AnimatorComponent;
	class Component;
	//class ColliderComponent; TODO

	
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager() = default;

		void onInit();

		void onLoad();

		const static void onUpdate(float dt);

		const static void ClearComponents();
	
		template< class T, typename... Args >
		const static void AddComponent(uint32_t entity, Args&&... params);
		
		template< class T>
		const static void AddComponent(const std::shared_ptr<Component>& component);

		template<class T>
		const static void RemoveComponent(const UUID& id);
		
		const static void RemoveComponentsbyID(const UUID& id);
		
		template<class T>
		const static void RemoveComponent(const std::shared_ptr<T>& component);

		template <class T>
		const static std::shared_ptr<T>GetComponent(const UUID& enitytId);
		
		template <class T>
		const static std::vector<std::shared_ptr<T>>GetAllComponents();

		template <class T>
		const static std::vector<std::shared_ptr<T>>GetComponents(const UUID& enitytId);

		template<class T>
		const static bool HasComponent(const UUID& id);
	
	private:
		
		static std::unique_ptr<ComponentSet> m_tagComponents;
		static std::unique_ptr<ComponentSet> m_RelationshipComponents;
		static std::unique_ptr<ComponentSet> m_TransformerComponents;
		static std::unique_ptr<ComponentSet> m_CameraControllerComponents;
		static std::unique_ptr<ComponentSet> m_RenderComponents;
		static std::unique_ptr<ComponentSet> m_LightComponents;
		static std::unique_ptr<ComponentSet> m_RigidBodyComponents;
		static std::unique_ptr<ComponentSet> m_BoxColliderComponents;
		static std::unique_ptr<ComponentSet> m_ScriptComponents;
		static std::unique_ptr<ComponentSet> m_BillboardComponents;
		
		static std::map<std::type_index, std::unique_ptr<ComponentSet>> Register;
	};

	template<class T, typename ...Args>
	inline const void  Fracture::ComponentManager::AddComponent(uint32_t entity, Args && ...params)
	{
		Register[typeid(T)]->AddComponent<T>(entity, params...);
	}

	template<class T>
	inline const void  Fracture::ComponentManager::AddComponent(const std::shared_ptr<Component>& component)
	{
		Register[typeid(T)]->AddComponent(component);
	}

	template<class T>
	inline const void  Fracture::ComponentManager::RemoveComponent(const UUID& id)
	{
		Register[typeid(T)]->RemoveComponent<T>(id);
	}

	template<class T>
	inline const void  Fracture::ComponentManager::RemoveComponent(const std::shared_ptr<T>& component)
	{
		Register[typeid(T)]->RemoveComponent(component);
	}

	template<class T>
	inline const std::shared_ptr<T> Fracture::ComponentManager::GetComponent(const UUID& entitytId)
	{
		std::shared_ptr<T> c = Register[typeid(T)]->GetComponent<T>(entitytId);
		if (c)
			return c;
		return nullptr;
	}

	template<class T>
	inline const std::vector<std::shared_ptr<T>>  Fracture::ComponentManager::GetAllComponents()
	{
		return Register[typeid(T)]->GetComponents<T>();
	}

	template<class T>
	inline const std::vector<std::shared_ptr<T>>  Fracture::ComponentManager::GetComponents(const UUID& entitytId)
	{
		std::vector<std::shared_ptr<T>> m_collection;
		const auto& components = Register[typeid(T)]->GetComponents(entitytId);

		for (auto& c : components)
		{
			auto new_c = std::dynamic_pointer_cast<T>(c);
			m_collection.push_back(new_c);
		}

		return m_collection;
	}


	template<class T>
	inline const bool Fracture::ComponentManager::HasComponent(const UUID& id)
	{
		ProfilerTimer timer("Has Component");
		if (Register[typeid(T)]->HasComponent<T>(id))
		{
			return true;
		}

		return false;
	}
}

#endif
