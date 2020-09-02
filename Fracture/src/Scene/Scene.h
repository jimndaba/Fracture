#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>

namespace Fracture
{

	class Entity;

	class Scene
	{

	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> Root;

		void addEntity(std::shared_ptr<Entity> entity);
		void removeEntity(std::shared_ptr<Entity> entity);
		void clearScene();

	private:

		std::shared_ptr<Entity> m_root = nullptr;
	};
}

#endif