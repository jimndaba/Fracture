#pragma once
#ifndef RELATIONSHIPCOMPONENT_H
#define RELATIONSHIPCOMPONENT_H

#include "Component.h"

namespace Fracture
{
	class Entity;

	class RelationShipComponent : public Component
	{

	public: 
		RelationShipComponent(UUID id);
		~RelationShipComponent() = default;


		RelationShipComponent(const RelationShipComponent& rel) :Component(rel)
		{
			hasParent = rel.hasParent;
			m_parent = rel.m_parent;
			noChildren = rel.noChildren;
			m_children = rel.m_children;
		}

		RelationShipComponent(const RelationShipComponent& component, UUID id) :Component(id)
		{
			hasParent = component.hasParent;
			if (hasParent)
			{
				m_parent = component.m_parent;
				SetParent(m_parent);
			}
			else
			{
				m_parent = 0;
				SetParent(m_parent);
			}
			noChildren = component.noChildren;
			m_children = component.m_children;
		}

		const RelationShipComponent& operator = (const RelationShipComponent& rel)
		{
			hasParent = rel.hasParent;
			m_parent = rel.m_parent;
			noChildren = rel.noChildren;
			m_children = rel.m_children;
			return *this;
		}

		virtual void onStart();

		uint32_t noChildren = 0;
		uint32_t m_parent;
		void SetParent(const UUID& parent);
		void ChangeParent(const UUID& parent);
		void AddChild(const UUID& child);
		void RemoveChild(const UUID& child);
		std::vector<UUID> GetChildren();

		bool hasChildren();
		bool hasParent = false;
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<RelationShipComponent> clone(UUID id) const
		{
			return std::shared_ptr<RelationShipComponent>(this->clone_impl(id));
		}


	private:

		virtual RelationShipComponent *clone_impl(UUID id) const override
		{
			return new RelationShipComponent(*this, id);
		}
		
		std::vector<UUID> m_children;
	};

}


#endif