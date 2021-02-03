#pragma once
#ifndef COMPONENTSET_H
#define COMPONENTSET_H

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <map>

namespace Fracture
{
	class Component;

	class ComponentSet
	{
	public:
		ComponentSet();

		template <typename name>
		bool HasComponent(uint32_t id);

		template <typename name>
		std::shared_ptr<name> GetComponent(uint32_t entitytId);


		template <class name, typename... Args>
		void AddComponent(uint32_t entity, Args && ...params);
		
		void AddComponent(const std::shared_ptr<Component>& component);
		
		template <typename name>
		std::vector<std::shared_ptr<name>> GetComponents();
		
		std::vector<std::shared_ptr<Component>> GetComponents(uint32_t id);

		void RemoveComponent(const std::shared_ptr<Component>& component);


		void RemoveComponent(uint32_t id);

		template <typename name>
		void RemoveComponent(uint32_t id);
		

		void Clear();

		std::vector<std::shared_ptr<Component>> Components();
		

	private:
		std::vector<std::shared_ptr<Component>> m_components;

	};

	template<typename name>
	inline bool Fracture::ComponentSet::HasComponent(uint32_t id)
	{
		std::shared_ptr<Component> c = GetComponent<name>(id);
		if (c)
			return true;

		return false;
	}

	template<typename name>
	inline std::shared_ptr<name> Fracture::ComponentSet::GetComponent(uint32_t entitytId)
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

	template<class name, typename ...Args>
	inline void Fracture::ComponentSet::AddComponent(uint32_t entity, Args && ...params)
	{
		std::shared_ptr<name>component = std::make_shared<name>(entity, params...);
		m_components.push_back(component);
	}

	template<typename name>
	inline  std::vector<std::shared_ptr<name>> Fracture::ComponentSet::GetComponents()
	{
		std::vector<std::shared_ptr<name>> set;

		for (std::shared_ptr<Component> component : m_components)
		{
			set.push_back(std::dynamic_pointer_cast<name>(component));
		}

		return set;
	}

	template <typename name>
	inline void Fracture::ComponentSet::RemoveComponent(uint32_t id)
	{
		std::shared_ptr<name> c = GetComponent<name>(id);
		if (c)
		{
			RemoveComponent(c);
		}
	}

}
#endif