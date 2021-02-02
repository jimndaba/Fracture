#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <string>
#include "GLAD/glad.h"
#include <glfw/glfw3.h>
namespace Fracture
{
	class GameWindow
	{

	public:
		GameWindow(int width, int height,std::string title);
		~GameWindow();

		void MaximiseWindow();

		void pollEvents();
		void swapBuffers();

		int Width;
		int Height;
		std::string Title;

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