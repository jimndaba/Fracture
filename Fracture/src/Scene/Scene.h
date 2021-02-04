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

		void addEntity(const std::shared_ptr<Entity>& entity);				
		void Destroy(const std::shared_ptr<Entity>& entity);
		void Destroy(uint32_t id);
		void Duplicate(const std::shared_ptr<Entity>& entity);
		void clearScene();		

		static std::shared_ptr<Entity> ActiveCamera();
		static void setCamera(std::shared_ptr<Entity> camera);

		std::vector<std::shared_ptr<Entity>> Entities();
		static std::shared_ptr<Entity> GetEntity(uint32_t id);
		std::string Name;

	private:
		std::shared_ptr<Entity> m_root;
		static std::shared_ptr<Entity> active_Camera;
		static std::vector<std::shared_ptr<Entity>> m_entities;

		template <class T>
		void CopyComponentIfExists(const std::shared_ptr<Entity>& copyTo,const std::shared_ptr<Entity>& copyFrom);
		
	};



}

#endif