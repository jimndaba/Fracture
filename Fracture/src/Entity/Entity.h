#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <algorithm>

namespace Fracture
{
	class Entity
	{
	public:
		Entity(int id);
		~Entity();

		int Id;

		void addChild(std::shared_ptr<Entity> child);
		void removeChild(std::shared_ptr<Entity> child);
		void clearChildren();
		void setParent(std::shared_ptr<Entity> parent);

		std::vector<std::shared_ptr<Entity>> Children();
		std::shared_ptr<Entity> Parent = nullptr;

	private:
		std::vector<std::shared_ptr<Entity>> m_children;

	};

}

#endif
