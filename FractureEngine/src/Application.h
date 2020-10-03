#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "Fracture.h"


namespace Fracture
{
	class Editor;

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

		std::unique_ptr<Game> m_Game;
		std::unique_ptr<Editor> m_Editor;
	};

}

#endif