#include "MouseState.h"
#include "Game/GameWindow.h"

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
	int state = glfwGetMouseButton(GameWindow::Context(), static_cast<int32_t>(key));
	if (state == GLFW_PRESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Fracture::Mouse::IsButtonUp(Fracture::MOUSECODE key)
{
	return false;
}

glm::vec2 Fracture::Mouse::GetPosition()
{
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);
	if (glfwGetWindowAttrib(GameWindow::Context(), GLFW_HOVERED))
	{
		double m_x, m_y;
		glfwGetCursorPos(GameWindow::Context(), &m_x, &m_y);
		pos.x = (float)m_x;
		pos.y = (float)m_y;
	}
	return pos;
}

bool Fracture::Mouse::IsScrolling()
{
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
