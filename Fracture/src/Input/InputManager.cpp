#include "InputManager.h"
#include <iostream>

SDL_Event Fracture::InputManager::event;

Fracture::InputManager::InputManager()
{
	
}

Fracture::InputManager::~InputManager()
{
}

bool Fracture::InputManager::IsKeyDown(KeyCode key)
{
	const Uint8* kb = SDL_GetKeyboardState(NULL);
	 if (kb[(SDL_Scancode)key])
	 {
		 return true;
	 }
	 return false;
}

bool Fracture::InputManager::IsMouseDown(MOUSECODE key)
{
	if (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON((Uint32)key))
	{
		return true;
	}
	return false;
}

glm::vec2 Fracture::InputManager::GetMousePosition()
{
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);	
	int x, y;
	SDL_GetMouseState(&x, &y);
	pos.x = (float)x;
	pos.y = (float)y;
	return pos;
}

bool Fracture::InputManager::IsMouseScroll()
{
	if (event.type == SDL_MOUSEWHEEL)
	{				
		return true;
	}
	return false;
}

glm::vec2 Fracture::InputManager::GetMouseScroll()
{
	return glm::vec2();
}

void Fracture::InputManager::PollEvents()
{
	SDL_PollEvent(&event);
}
