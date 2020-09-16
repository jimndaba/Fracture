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


	class Game
	{

	public:
		Game();
		~Game();

		void run();
		void init();
		void loadContent();
		void update();
		void render();
		void unloadContent();
		void shutdown();

		//events
		void onQuit();
		void onWindowResize(int width, int height);

	private:
		bool m_isRunning = true;
		std::unique_ptr<GameWindow> m_GameWindow;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<AssetManager> m_AssetManager;
		std::unique_ptr<EntityManager> m_EntityManager;
	};

}
#endif // ! GAME_H

