#include "InputManager.h"
#include <iostream>
#include "Logging/Logger.h"

Fracture::Mouse Fracture::InputManager::m_mouse;
Fracture::KeyboardState Fracture::InputManager::m_keyState;


Fracture::InputManager::InputManager()
{
	glfwSetScrollCallback(GameWindow::Context(), scroll_callback);
}

Fracture::InputManager::~InputManager()
{
}

bool Fracture::InputManager::IsKeyDown(KeyCode key)
{
	int state = glfwGetKey(GameWindow::Context(), static_cast<int32_t>(key));
	if (state == GLFW_PRESS && state != GLFW_REPEAT)
		return true;
	else
		return false;
}

bool Fracture::InputManager::IsMouseDown(MOUSECODE key)
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

bool Fracture::InputManager::IsMouseUp(MOUSECODE key)
{
	return false;
}

glm::vec2 Fracture::InputManager::GetMousePosition()
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

bool Fracture::InputManager::IsMouseScroll()
{
	return isScrolling;
}

glm::vec2 Fracture::InputManager::GetMouseScroll()
{
	if (isScrolling)
	{
		isScrolling = false;
		return m_scroll;
	}
	else
	{
		return glm::vec2(0.0f);
	}
	isScrolling = false;
}

bool Fracture::InputManager::GetMouseState()
{
	return false;
}

Fracture::Mouse Fracture::InputManager::GetMouse()
{
	return m_mouse;
}

void Fracture::InputManager::PollEvents()
{
	glfwPollEvents();
}

void Fracture::InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	isScrolling = true;
	m_scroll.x = (float)xoffset;
	m_scroll.y = (float)yoffset;
}
