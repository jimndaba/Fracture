#include "MouseState.h"

SDL_Event Fracture::Mouse::event;
Fracture::MouseState Fracture::Mouse::m_MouseState;

Fracture::Mouse::Mouse()
{
	m_MouseState = MouseState::Up;
}

Fracture::Mouse::~Mouse()
{
}

bool Fracture::Mouse::IsButtonDown(Fracture::MOUSECODE key)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON((Uint32)key))
	{
		m_MouseState = MouseState::Up;
		return true;
	}
	m_MouseState = MouseState::Down;
	return false;
}

bool Fracture::Mouse::IsButtonUp(Fracture::MOUSECODE key)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON((Uint32)key))
	{
		m_MouseState = MouseState::Up;
		return true;
	}
	m_MouseState = MouseState::Down;
	return false;
}

glm::vec2 Fracture::Mouse::GetPosition()
{
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);
	int x, y;
	SDL_GetMouseState(&x, &y);
	pos.x = (float)x;
	pos.y = (float)y;
	return pos;
}

bool Fracture::Mouse::IsScrolling()
{
	if (event.type == SDL_MOUSEWHEEL)
	{
		return true;
	}
	return false;
}

glm::vec2 Fracture::Mouse::GetScroll()
{
    return glm::vec2();
}

Fracture::MouseState Fracture::Mouse::GetState()
{
    return m_MouseState;
}
