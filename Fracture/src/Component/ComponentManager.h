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

namespace Fracture
{
	
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		void onUpdate(float dt);
				
		template< class T, typename... Args >
		static void AddComponent(int entity, Args&&... params);

		template <class T>
		static std::shared_ptr<T>GetComponent(int enitytId);	

		static std::vector<std::shared_ptr<Component>> GetComponents(int enitytId);

	
	private:
		static std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T, typename ...Args>
	inline void ComponentManager::AddComponent(int entity, Args && ...params)
	{		
		m_Components.push_back(std::make_shared<T>(entity,params...));
	}

	template<class T>
	inline std::shared_ptr<T> Fracture::ComponentManager::GetComponent(int entitytId)
	{
		for (const auto& component : m_Components)
		{						
				if (component != nullptr && component->entityID == entitytId && std::dynamic_pointer_cast<T>(component) != nullptr)
				{						
					return std::dynamic_pointer_cast<T>(component);
				}							
		}
		return nullptr;
	}

	
}

#endif
