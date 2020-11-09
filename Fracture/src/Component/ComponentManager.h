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
#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"
#include "Physics/PhysicsManager.h"

namespace Fracture
{
	
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		void onLoad();

		static void onUpdate(float dt);

		static void onDebugDraw();

		static void ClearComponents();
	
		template< class T, typename... Args >
		static void AddComponent(uint32_t entity, Args&&... params);
		static void AddComponent(std::shared_ptr<Component> component);

		template<class T>
		static void RemoveComponent(uint32_t id);
		static void RemoveComponentsbyID(uint32_t id);
		static void RemoveComponent(std::shared_ptr<Component> component);

		template <class T>
		static std::shared_ptr<T>GetComponent(uint32_t enitytId);
		static std::vector<std::shared_ptr<Component>> GetComponents(uint32_t enitytId);
		static std::vector<std::shared_ptr<Component>> GetAllComponents();

		template<class T>
		static bool HasComponent(uint32_t id);
	
	private:
		static std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T, typename ...Args>
	inline void ComponentManager::AddComponent(uint32_t entity, Args && ...params)
	{		
		m_Components.push_back(std::make_shared<T>(entity, params...));
	}

	template<class T>
	inline void ComponentManager::RemoveComponent(uint32_t id)
	{
		std::vector<std::shared_ptr<Component>>::const_iterator it;
		for (it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it)->EntityID == id && std::dynamic_pointer_cast<T>(*it) != nullptr)
			{
				FRACTURE_TRACE("Removed component: {} of Type {}", (*it)->EntityID, (*it)->componentType);
				RemoveComponent(*it);				
			}
			
		}
	} 

	template<class T>
	inline std::shared_ptr<T> Fracture::ComponentManager::GetComponent(uint32_t entitytId)
	{
		for (const auto& component : m_Components)
		{						
				if (component != nullptr && component->EntityID == entitytId && std::dynamic_pointer_cast<T>(component) != nullptr)
				{						
					return std::dynamic_pointer_cast<T>(component);
				}							
		}
		return nullptr;
	}

	template<class T>
	inline bool ComponentManager::HasComponent(uint32_t id)
	{
		for (auto& component : m_Components)
		{
			std::shared_ptr<T> c = GetComponent<T>(id);

			if (c)
				return true;
		}
		return false;
	}

	
}

#endif
