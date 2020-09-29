#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

namespace Fracture
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		int Id;
		std::string name;

		void addChild(std::shared_ptr<Entity> child);
		void removeChild(std::shared_ptr<Entity> child);
		void clearChildren();
		void setParent(std::shared_ptr<Entity> parent);

		std::shared_ptr<Entity> GetParent();

		std::vector<std::shared_ptr<Entity>> Children();
		std::shared_ptr<Entity> Parent = nullptr;		
	private:
		std::vector<std::shared_ptr<Entity>> m_children;

	};

	class EntityInstance
	{
	public:
		unsigned int EntityID;
		unsigned int InstanceID;
	};

}

#endif
