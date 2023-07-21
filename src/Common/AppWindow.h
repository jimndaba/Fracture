#pragma once
#ifndef APPWINDOW_H
#define APPWINDOW_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Fracture
{
	struct WindowCreationInfo
	{
		const char* Name;
		uint32_t Width;
		uint32_t Height;
		bool IsVsync = true;
		bool IsResizable = true;
		bool IsFullscreen = false;
		bool IsMaximised = true;
		bool IsBordered = true;	
		GLFWwindow* mSharedContext = nullptr;
	};

	class AppWindow
	{
	public:
		AppWindow(const WindowCreationInfo& info);
		~AppWindow();

		void Init();
		void SwapBuffers();
		void PollEvents(bool waitforInput = false);
		bool ShouldWindowClose();	
		void MakeCurrentContext();
		void SetWindowTitle(const std::string& title);
		
		WindowCreationInfo Info = WindowCreationInfo{};
		GLFWwindow* Context;

		static void OnWindowResize(GLFWwindow* window, int width, int height);
		static void OnMinimised(GLFWwindow* window, int iconified);

	private:
	

	};


}

#endif