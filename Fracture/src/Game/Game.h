#pragma once
#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>

namespace Fracture
{

	class GameWindow;
	class Renderer;
	class ComponentManager;
	class AssetManager;
	class EntityManager;
	class Entity;
	class InputManager;
	class Scene;
	//class ScriptManager;
	class GameLogic;
	class PhysicsManager;
	class Logger;
	//class DebugRenderer;
	class Eventbus;
	class SceneManager;
	struct Event;
	struct GameSettings;
	struct ProjectProperties;
	class FrameGraph;

	
	class Game
	{

	public:
		Game();	
		~Game();

		void run();
		void init();
		void loadContent();
		void update(float dt);
		void render();
		void unloadContent();
		void shutdown();
		void onQuit();

		//scene management
		static void addScene(std::string name, std::shared_ptr<Fracture::Scene> scene);
		static void addEntity(std::shared_ptr<Fracture::Entity> entity);
		static void removeScene(std::string name);
		static void changeScene(std::string name);
		std::shared_ptr<Fracture::Scene> CurrentScene();
	
		//Scripting
		static void AddScript(std::shared_ptr<GameLogic> script);
		
		//events
		static std::shared_ptr<Eventbus> GetEventbus();
		static void onEvent(Event* mEvent);
		void onWindowResize(int width, int height);

	private:
		bool m_isRunning = true;
		std::shared_ptr<ProjectProperties> m_properties;
		std::unique_ptr<GameWindow> m_GameWindow;
		std::shared_ptr<Renderer> m_Renderer;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<AssetManager> m_AssetManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<PhysicsManager> m_PhysicsManager;
		std::unique_ptr<Logger> m_logger;
		std::shared_ptr<FrameGraph> m_framegraph;
		//std::unique_ptr<DebugRenderer> m_debug;
		std::shared_ptr<GameSettings> m_GameSettings;
		static std::unique_ptr<SceneManager> m_SceneManager;
		static std::shared_ptr<Eventbus> m_Eventbus;
		static std::shared_ptr<Fracture::Scene> m_currentScene;
		//static std::unique_ptr<ScriptManager> m_ScriptManager;

	};

}
#endif

