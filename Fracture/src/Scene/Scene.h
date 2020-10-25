#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>


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


		virtual void onLoad() {};

		void addEntity(std::shared_ptr<Entity> entity);				
		void Destroy(std::shared_ptr<Entity> entity);
		void Destroy(uint32_t id);
		void clearScene();		

		static std::shared_ptr<Entity> MainCamera();
		std::vector<std::shared_ptr<Entity>> Entities();

		static std::shared_ptr<Entity> GetEntity(uint32_t id);

	private:
		std::shared_ptr<Entity> m_root;
		static std::shared_ptr<Entity> main_Camera;
		static std::vector<std::shared_ptr<Entity>> m_entities;
		
	};
}

#endif