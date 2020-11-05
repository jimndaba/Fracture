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

		void Instance(std::shared_ptr<Entity> entity);
		void Instance(std::shared_ptr<Entity> entity,glm::vec3 pos);
		void Instance(std::shared_ptr<Entity> entity, glm::vec3 position, glm::vec3 scale);
		void Instance(std::shared_ptr<Entity> entity, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

		void addEntity(std::shared_ptr<Entity> entity);				
		void Destroy(std::shared_ptr<Entity> entity);
		void Destroy(uint32_t id);
		void clearScene();		

		static std::shared_ptr<Entity> MainCamera();
		static void setCamera(std::shared_ptr<Entity> camera);

		std::vector<std::shared_ptr<Entity>> Entities();

		static std::shared_ptr<Entity> GetEntity(uint32_t id);

		std::string Name;

	private:
		std::shared_ptr<Entity> m_root;
		std::shared_ptr<Entity> main_sunlight;
		static std::shared_ptr<Entity> main_Camera;
		static std::vector<std::shared_ptr<Entity>> m_entities;
		
	};
}

#endif