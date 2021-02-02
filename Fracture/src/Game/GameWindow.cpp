#include "GameWindow.h"

#include "Game/Game.h"
#include "Event/Event.h"



GLFWwindow* Fracture::GameWindow::window;

Fracture::GameWindow::GameWindow(int width, int height, std::string title):
	Width(width),Height(height),Title(title)
{

	if (!glfwInit())
	{
		//FRACTURE_CRITICAL("FAILED TO INIT SDL");
		glfwTerminate();
		return;
	}


	//if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	//	
	//	return;
	//}

	//SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//
	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
	//
	//
	//
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);



	//SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

	//m_window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, flags);
	//if (m_window == nullptr) {
	//	FRACTURE_CRITICAL("FAILED TO INIT WINDOW : {}" , SDL_GetError());
	//	glfwTerminate();;
	//	return;
	//}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{
		//const char** error;
		//glfwGetError(error);
		//FRACTURE_CRITICAL("FAILED TO create OpenGL context {}", error);
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwSwapInterval(0);
	glfwMakeContextCurrent(window);


	//maincontext = SDL_GL_CreateContext(m_window);
	//if (maincontext == NULL)
	//{
	//	FRACTURE_CRITICAL("FAILED TO create OpenGL context {}" ,SDL_GetError());
	//}
		

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// Check OpenGL properties
	//FRACTURE_INFO("OpenGL loaded");
	//FRACTURE_INFO("Vendor: {}", glGetString(GL_VENDOR));
	//FRACTURE_INFO("Renderer: {}", glGetString(GL_RENDERER));
	//FRACTURE_INFO("Version: {}", glGetString(GL_VERSION));
}

Fracture::GameWindow::~GameWindow()
{
	glfwTerminate();
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
