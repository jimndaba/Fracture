#pragma once
#ifndef GAME_H
#define GAME_H

#include <memory>

namespace Fracture
{

	class GameWindow;
	class Renderer;
	class ComponentManager;
	class AssetManager;
	class EntityManager;
	class InputManager;
	class IDManager;
	class Scene;
	class ScriptManager;
	class GameLogic;
	class PhysicsManager;
	class Logger;

	class Game
	{

	public:
		Game();
		Game(int width, int height);
		~Game();

		void run();
		void init();
		void loadContent();
		void update(float dt);
		void render();
		void unloadContent();
		void shutdown();

		//scene management
		void addScene(std::shared_ptr<Fracture::Scene> scene);
		//void removeScene();
		//void changeScene();
		std::shared_ptr<Fracture::Scene> CurrentScene();

		//Scripting
		static void AddScript(std::shared_ptr<GameLogic> script);

		//events
		void onQuit();
		void onWindowResize(int width, int height);

		Renderer* GetRenderer();
	
	private:
		bool m_isRunning = true;
		std::unique_ptr<GameWindow> m_GameWindow;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<AssetManager> m_AssetManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<InputManager> m_InputManager;
		std::unique_ptr<IDManager> m_IDManager;
		std::unique_ptr<PhysicsManager> m_PhysicsManager;
		std::unique_ptr<Logger> m_logger;
	
		std::shared_ptr<Fracture::Scene> m_currentScene;
		static std::unique_ptr<ScriptManager> m_ScriptManager;
	};

}
#endif

