#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <map>
#include <memory>
#include <string>

namespace Fracture
{

	class Scene;
	class Entity;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		static void AddScene(std::string name, std::shared_ptr<Scene> scene);
		static void RemoveScene(std::string name);

		static void SetScene(std::string name);
		static void SetSceneName(std::shared_ptr<Scene> scene,std::string name);
		static std::shared_ptr<Scene> GetActiveScene();

		static void AddEntity(std::shared_ptr<Entity> entity);
		static void RemoveEntity(std::shared_ptr<Entity> entity);
		static void RemoveEntity(uint32_t entity);
		static std::shared_ptr<Entity> getEntity(uint32_t id);

	private:

		static std::map<std::string,std::shared_ptr<Scene>> m_scenes;
		static std::shared_ptr<Scene> m_activeScene;
	};

}

#endif