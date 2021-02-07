#include "GameWindow.h"
#include "Game/Game.h"
#include "Event/Event.h"
#include "Logging/Logger.h"
#include "GLAD/glad.h"


GLFWwindow* Fracture::GameWindow::window;

Fracture::GameWindow::GameWindow(int width, int height, std::string title):
	Width(width),Height(height),Title(title)
{

	if (!glfwInit())
	{
		FRACTURE_CRITICAL("FAILED TO INIT SDL");
		glfwTerminate();
	return;
	}

	

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{	
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

		
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FRACTURE_ERROR("Failed to initialize GLAD");
		return;
	}

	glfwSwapInterval(1);
		
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
	
	/*
	while (SDL_PollEvent(&m_event))
	{
		InputManager::PollEvents(m_event);

		if (m_event.type == SDL_QUIT)
		{
			game.onQuit();
		}
		if (m_event.type == SDL_WINDOWEVENT)
		{
			switch (m_event.window.event) {
            case SDL_WINDOWEVENT_SHOWN:              
                break;
            case SDL_WINDOWEVENT_HIDDEN:              
                break;
            case SDL_WINDOWEVENT_EXPOSED:               
                break;
            case SDL_WINDOWEVENT_MOVED:                
                break;
            case SDL_WINDOWEVENT_RESIZED:
                //game.onWindowResize(SDL_GetWindowSurface(m_window)->w, SDL_GetWindowSurface(m_window)->h);
				Game::onEvent(new WindowResizeEvent(SDL_GetWindowSurface(m_window)->w, SDL_GetWindowSurface(m_window)->h));
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
				FRACTURE_TRACE("Window {} size changed to {}x{}",
                    m_event.window.windowID, m_event.window.data1,
                    m_event.window.data2);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                break;
            case SDL_WINDOWEVENT_RESTORED:
                break;
            case SDL_WINDOWEVENT_ENTER:              
                break;
            case SDL_WINDOWEVENT_LEAVE:
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
               break;
            case SDL_WINDOWEVENT_CLOSE:
				game.onQuit();
              break;			
			}

			
		}

	}
	*/
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
	glfwDestroyWindow(window);
}
