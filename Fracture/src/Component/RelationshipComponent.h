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
	private:
		
		std::vector<uint32_t> m_children;
	};

}


#endif