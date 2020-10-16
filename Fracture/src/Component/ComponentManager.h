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

		void onLoad();

		void onUpdate(float dt);
	
		//AddRenderComponent (ID, model , material)
		//AddTransformComponent (ID)
		//AddTransformComponent (ID, position)
		//AddTransformComponent (ID, positon , scale)
		//AddTransformComponent (ID, positon , scale, rotation)
		//AddScriptComponent (ID, script)
		template< class T, typename... Args >
		static void AddComponent(uint32_t entity, Args&&... params);

		static void AddComponent(std::shared_ptr<Component> component);

		template <class T>
		static std::shared_ptr<T>GetComponent(uint32_t enitytId);

		static std::vector<std::shared_ptr<Component>> GetComponents(uint32_t enitytId);

	
	private:
		static std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T, typename ...Args>
	inline void ComponentManager::AddComponent(uint32_t entity, Args && ...params)
	{		
		m_Components.push_back(std::make_shared<T>(entity,params...));
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

	
}

#endif
