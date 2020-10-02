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
	class Camera;

	class Scene
	{

	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> Root();


		virtual void onLoad() = 0;

		void addEntity(std::shared_ptr<Entity> entity);		
		void removeEntity(std::shared_ptr<Entity> entity);
		void clearScene();		

		static std::shared_ptr<Camera> MainCamera();

	private:
		std::shared_ptr<Entity> m_root;
		static std::shared_ptr<Camera> main_Camera;
		
	};
}

#endif