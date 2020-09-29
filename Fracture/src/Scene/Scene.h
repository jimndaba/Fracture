#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#include "Entity/Camera.h"

namespace Fracture
{

	class Entity; 
	class TransformComponent;
	class InstancePool;

	class Scene
	{

	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> Root();

		void addEntity(std::shared_ptr<Entity> entity);		
		void removeEntity(std::shared_ptr<Entity> entity);

		void Instantiate(std::shared_ptr<Entity> entity);
		void Instantiate(std::shared_ptr<Entity> entity,glm::vec3 position);

		void clearScene();

		void onLoad();

		std::map<std::string, std::shared_ptr<InstancePool>>* GetInstances();

		static std::shared_ptr<Camera> MainCamera();

	private:
		std::shared_ptr<Entity> m_root;
		static std::shared_ptr<Camera> main_Camera;
		std::map<std::string,std::shared_ptr<InstancePool>>* m_instancepools;
	};
}

#endif