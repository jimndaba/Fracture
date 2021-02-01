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
#include "Component.h"
#include "Logging/Logger.h"
#include "Physics/PhysicsManager.h"
#include "Profiling/Profiler.h"


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
	//class ColliderComponent; TODO

	
	class ComponentSet
	{
	public:
		ComponentSet() {
		
		}

		template <typename name>
		bool HasComponent(uint32_t id)
		{
			std::shared_ptr<Component> c = GetComponent<name>(id);
			if (c)
				return true;

			return false;
		}

		template <typename name>
		std::shared_ptr<name> GetComponent(uint32_t entitytId)
		{
			for (std::shared_ptr<Component> component : m_components)
			{
				if (component != nullptr && component->EntityID == entitytId)
				{
					return std::dynamic_pointer_cast<name>(component);
				}
			}
			return nullptr;
		}

		template <class name,typename... Args>
		void AddComponent(uint32_t entity, Args && ...params)
		{
			std::shared_ptr<name>component = std::make_shared<name>(entity, params...);
			m_components.push_back(component);
		}

		void AddComponent(const std::shared_ptr<Component>& component)
		{
			m_components.push_back(component);
		}

		template <typename name>
		std::vector<std::shared_ptr<name>> GetComponents()
		{
			std::vector<std::shared_ptr<name>> set;

			for (std::shared_ptr<Component> component : m_components)
			{
				set.push_back(std::dynamic_pointer_cast<name>(component));
			}

			return set;
		}
				
		std::vector<std::shared_ptr<Component>> GetComponents(uint32_t id)
		{
			std::vector<std::shared_ptr<Component>>  set;

			if (!m_components.empty())
			{
				for (std::shared_ptr<Component> component : m_components)
				{
					if (component != NULL && component->EntityID == id)
					{
						set.push_back(component);
					}
				}
			}			
			return set;
		}
		
		void RemoveComponent(const std::shared_ptr<Component>& component)
		{
			if (!m_components.empty())
			{
				auto it = std::find_if(std::begin(m_components), std::end(m_components), [component](std::shared_ptr<Component>& p) { return p == component; });
				if (it != m_components.end())
				{
					m_components.erase(
						std::remove(std::begin(m_components),
							std::end(m_components), component),
						std::end(m_components));

				}
			}
			else
			{
				FRACTURE_ERROR("COMPONENT SET OF TYPE: {} IS EMPTY", typeid(Component).name());
			}
		}

		void RemoveComponent(uint32_t id)
		{
			for (auto& component : m_components)
			{
				if (component != nullptr && component->EntityID == id)
				{
					RemoveComponent(component);
				}
			}
		}

		template <typename name>
		void RemoveComponent(uint32_t id)
		{
			std::shared_ptr<name> c = GetComponent<name>(id);
			if (c)
			{
				RemoveComponent(c);
			}

		}		

		void Clear()
		{
			m_components.clear();
		}
				

		std::vector<std::shared_ptr<Component>> Components()
		{
			return m_components;
		}
		
	private:
		std::vector<std::shared_ptr<Component>> m_components;

	};
		

	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager() = default;

		void onInit();

		void onLoad();

		static void onUpdate(float dt);

		static void ClearComponents();
	
		template< class T, typename... Args >
		static void AddComponent(uint32_t entity, Args&&... params);
		
		template< class T>
		static void AddComponent(const std::shared_ptr<Component>& component);

		template<class T>
		static void RemoveComponent(const uint32_t& id);
		
		static void RemoveComponentsbyID(const uint32_t& id);
		
		template<class T>
		static void RemoveComponent(const std::shared_ptr<T>& component);

		template <class T>
		static std::shared_ptr<T>GetComponent(uint32_t enitytId);
		
		template <class T>
		static std::vector<std::shared_ptr<T>>GetAllComponents();

		template <class T>
		static std::vector<std::shared_ptr<T>>GetComponents(uint32_t enitytId);

		template<class T>
		static bool HasComponent(uint32_t id);
	
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
	inline void ComponentManager::AddComponent(uint32_t entity, Args && ...params)
	{				
		Register[typeid(T)]->AddComponent<T>(entity, params...);
	}

	template<class T>
	inline void ComponentManager::AddComponent(const std::shared_ptr<Component>& component)
	{
		Register[typeid(T)]->AddComponent(component);
	}


	template<class T>
	inline void ComponentManager::RemoveComponent(const uint32_t& id)
	{
		Register[typeid(T)]->RemoveComponent<T>(id);
	} 

	template<class T>
	inline void ComponentManager::RemoveComponent(const std::shared_ptr<T>& component)
	{
		Register[typeid(T)]->RemoveComponent(component);
	}

	template<class T>
	inline std::shared_ptr<T> Fracture::ComponentManager::GetComponent(uint32_t entitytId)
	{
		std::shared_ptr<T> c = Register[typeid(T)]->GetComponent<T>(entitytId);
		if(c)
			return c;
		return nullptr;
	}

	template<class T>
	inline std::vector<std::shared_ptr<T>> ComponentManager::GetAllComponents()
	{
		return Register[typeid(T)]->GetComponents<T>();
	}
	
	template<class T>
	inline std::vector<std::shared_ptr<T>> ComponentManager::GetComponents(uint32_t entitytId)
	{		
		return Register[typeid(T)]->GetComponents(entitytId);
	}

	template<class T>
	inline bool ComponentManager::HasComponent(uint32_t id)
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
