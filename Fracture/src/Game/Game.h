#pragma once
#ifndef GAME_H
#define GAME_H

#include <memory>

namespace Fracture
{

	class GameWindow;
	class Renderer;

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
		std::shared_ptr<GameWindow> m_GameWindow;
		std::unique_ptr<Renderer> m_Renderer;
	};

}
#endif // ! GAME_H

