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
		RelationShipComponent(uint32_t id);
		~RelationShipComponent() = default;


		RelationShipComponent(const RelationShipComponent& rel) :Component(rel.EntityID,ComponentType::Relationship)
		{
			hasParent = rel.hasParent;
			m_parent = rel.m_parent;
			noChildren = rel.noChildren;
			m_children = rel.m_children;
		}

		RelationShipComponent(const RelationShipComponent& component, uint32_t id) :Component(id, ComponentType::Relationship)
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
		void SetParent(const uint32_t& parent);
		void ChangeParent(const uint32_t& parent);
		void AddChild(const uint32_t& child);
		void RemoveChild(const uint32_t& child);
		std::vector<uint32_t> GetChildren();

		bool hasChildren();
		bool hasParent = false;
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<RelationShipComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<RelationShipComponent>(this->clone_impl(entityID));
		}


	private:

		virtual RelationShipComponent *clone_impl(uint32_t entityID) const override
		{
			return new RelationShipComponent(*this, entityID);
		}
		
		std::vector<uint32_t> m_children;
	};

}


#endif