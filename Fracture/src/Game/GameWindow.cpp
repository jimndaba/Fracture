#include "GameWindow.h"
#include "Game/Game.h"

Fracture::GameWindow::GameWindow(int width, int height, std::string title):
	Width(width),Height(height),Title(title)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.
		//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	m_window = SDL_CreateWindow(Title.c_str(), 100, 100, Width, Height, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (m_window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	maincontext = SDL_GL_CreateContext(m_window);
	if (maincontext == NULL)
	{
		std::cout << ("Failed to create OpenGL context") << SDL_GetError() << std::endl;
	}
		

	gladLoadGLLoader(SDL_GL_GetProcAddress);
	// Check OpenGL properties
	printf("OpenGL loaded\n");
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
}

Fracture::GameWindow::~GameWindow()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Fracture::GameWindow::pollEvents(Game& game)
{
	
	while (SDL_PollEvent(&m_event))
	{
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
                game.onWindowResize(SDL_GetWindowSurface(m_window)->w, SDL_GetWindowSurface(m_window)->h);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log("Window %d size changed to %dx%d",
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
}

void Fracture::GameWindow::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}
