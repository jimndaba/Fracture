#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "Fracture.h"

namespace Fracture
{
	class Editor;
	class Game;

	enum class EngineState
	{
		Editor,
		Game,
		Paused,
	};

	class Application
	{

	public:
		Application();
		~Application();

		void init();
		void run();
		void onUpdate();
		void onDraw();

	private:

		bool m_IsRunning = true;
		EngineState m_state;
		std::unique_ptr<Editor> m_Editor;
		std::unique_ptr<Game> m_Game;
		//EventQueue
	};

}

#endif