#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Fracture
{
	class GameWindow
	{

	public:
		GameWindow(int width, int height,std::string title, bool resizable = true);
		~GameWindow() = default;

		void MaximiseWindow();

		void pollEvents();
		void swapBuffers();

		int Width;
		int Height;
		std::string Title;
		bool IsResizable;
		bool IsFullscreen;

		static GLFWwindow* Context()
		{
			return window;
		}
				
		bool ShouldClose();
		void close();

	private:
		static GLFWwindow* window;
	};
}

#endif