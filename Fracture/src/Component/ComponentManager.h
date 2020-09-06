#pragma once
#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H


#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include "Component.h"

namespace Fracture
{

	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		void onUpdate();

		static void AddComponent(std::shared_ptr<Component> component);
		static void RemoveComponent(std::shared_ptr<Component> component);

		template <class T>
		static std::shared_ptr<T>	GetComponent(int enitytId, ComponentType compType);

	private:
		static std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T>
	inline std::shared_ptr<T> Fracture::ComponentManager::GetComponent(int entitytId,ComponentType compType)
	{
		
		for (const auto& component_pair : m_Components)
		{
			if (component_pair != nullptr && component_pair->entityID == entitytId && component_pair->componentType == compType)
			{
				return std::dynamic_pointer_cast<T>(component_pair);
			}
		}
		return nullptr;
	}


}

#endif
