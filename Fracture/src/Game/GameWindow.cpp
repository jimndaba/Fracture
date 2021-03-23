#include "GameWindow.h"
#include "Core/FractureCore.h"
#include "Game/Game.h"
#include "Event/Event.h"
#include "Logging/Logger.h"
#include "GLAD/glad.h"


GLFWwindow* Fracture::GameWindow::window;

Fracture::GameWindow::GameWindow(int width, int height, std::string title, bool resizable):
	Width(width),Height(height),Title(title),IsResizable(resizable)
{

	if (!glfwInit())
	{
		FRACTURE_CRITICAL("FAILED TO INIT SDL");
		glfwTerminate();
		return;
	}

	if (IsResizable)
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	}
	else
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8); // 4x antialiasing
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	std::string fulltitle =title + " - "+  "Fracture Engine: " +Fracture_Verion;
	window = glfwCreateWindow(width, height, fulltitle.c_str(), NULL, NULL);
	if (!window)
	{	
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
			
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FRACTURE_ERROR("Failed to initialize GLAD");
		return;
	}

	glfwSwapInterval(0);
		
	// Check OpenGL properties
	FRACTURE_INFO("OpenGL loaded");
	FRACTURE_INFO("Vendor: {}", glGetString(GL_VENDOR));
	FRACTURE_INFO("Renderer: {}", glGetString(GL_RENDERER));
	FRACTURE_INFO("Version: {}", glGetString(GL_VERSION));
}

void Fracture::GameWindow::MaximiseWindow()
{
	glfwMaximizeWindow(window);
}

void Fracture::GameWindow::pollEvents()
{
	glfwPollEvents();
}

void Fracture::GameWindow::swapBuffers()
{
	glfwSwapBuffers(window);	
}

bool Fracture::GameWindow::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Fracture::GameWindow::close()
{
	glfwTerminate();
	//glfwDestroyWindow(window);
}

std::shared_ptr<Fracture::GameWindow> Fracture::GameWindow::Create(int width, int height, std::string title, bool resizable)
{
	return std::make_shared<GameWindow>(width,height,title,resizable);
}
