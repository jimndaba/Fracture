#include "ComponentSet.h"
#include "Component.h"
#include "Logging/Logger.h"

Fracture::ComponentSet::ComponentSet()
{
}



void Fracture::ComponentSet::AddComponent(const std::shared_ptr<Component>& component)
{
	m_components.push_back(component);
}

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentSet::GetComponents(uint32_t id)
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

void Fracture::ComponentSet::Clear()
{
	m_components.clear();
}

std::vector<std::shared_ptr<Fracture::Component>> Fracture::ComponentSet::Components()
{
	return m_components;
}

void Fracture::ComponentSet::RemoveComponent(const std::shared_ptr<Fracture::Component>& component)
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
		FRACTURE_ERROR("COMPONENT SET OF TYPE: {} IS EMPTY",(void*)typeid(Component).name());
	}
}

void Fracture::ComponentSet::RemoveComponent(uint32_t id)
{
	for (auto& component : m_components)
	{
		if (component != nullptr && component->EntityID == id)
		{
			RemoveComponent(component);
		}
	}
}

