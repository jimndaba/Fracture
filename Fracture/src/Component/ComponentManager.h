#pragma once
#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H


#include <vector>
#include <memory>
#include <algorithm>

namespace Fracture
{
	class Component;

	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		void onUpdate();

		static void AddComponent(std::shared_ptr<Component> component);
		static void RemoveComponent(std::shared_ptr<Component> component);

		template <class T>
		static std::shared_ptr<T>	GetComponent(int enitytId);

	private:
		static std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<class T>
	inline std::shared_ptr<T> Fracture::ComponentManager::GetComponent(int entitytId)
	{
		for (const auto& component_pair : m_Components)
		{
			if (std::static_pointer_cast<T>(component_pair) != nullptr && component_pair->entityID == entitytId)
			{
				return std::static_pointer_cast<T>(component_pair);			
			}
		}
	}


}

#endif
