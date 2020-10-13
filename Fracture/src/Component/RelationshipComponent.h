#pragma once
#ifndef RELATIONSHIPCOMPONENT_H
#define RELATIONSHIPCOMPONENT_H

#include "Component.h"
#include <memory>
#include <vector>

namespace Fracture
{
	class Entity;

	class RelationShipComponent : public Component
	{

	public: 
		RelationShipComponent(uint32_t id);
		~RelationShipComponent();

		virtual void onStart();

		uint32_t noChildren = 0;
		std::shared_ptr<Entity> m_parent;
		void SetParent(std::shared_ptr<Entity> parent);
	private:
		

	};

}


#endif