#include "FracturePCH.h"
#include "Common/Logger.h"
#include "Input.h"
#include "Common/AppWindow.h"
#include "Common/FractureApp.h"


glm::vec2 Fracture::Input::m_scroll;
std::map<std::string, Fracture::KeyCode> Fracture::Input::mKeyMapping;

Fracture::Input::Input()
{
	glfwSetScrollCallback(IFractureApp::mWindow->Context, Fracture::Input::scroll_callback);
	glfwSetCursorPosCallback(IFractureApp::mWindow->Context, cursor_position_callback);
	glfwSetMouseButtonCallback(IFractureApp::mWindow->Context, mouse_button_callback);
	glfwSetKeyCallback(IFractureApp::mWindow->Context, key_callback);
}

void Fracture::Input::Update()
{
}

bool Fracture::Input::IsKeyDown(KeyCode key)
{
	int state = glfwGetKey(IFractureApp::mWindow->Context, static_cast<int32_t>(key));
	if (state == GLFW_PRESS || state == GLFW_REPEAT)
		return true;
	else
		return false;
}

bool Fracture::Input::IsKeyUp(KeyCode key)
{
	return false;
}

bool Fracture::Input::KeyPressed(KeyCode key)
{
	int state = glfwGetKey(IFractureApp::mWindow->Context, static_cast<int32_t>(key));
	if (state == GLFW_PRESS && state != GLFW_REPEAT)
		return true;
	else
		return false;
}

bool Fracture::Input::KeyReleased(KeyCode key)
{
	int state = glfwGetKey(IFractureApp::mWindow->Context, static_cast<int32_t>(key));
	if (state == GLFW_RELEASE )
		return true;
	else
		return false;
}

bool Fracture::Input::IsMouseDown(MouseCode btn)
{
	int state = glfwGetMouseButton(IFractureApp::mWindow->Context, static_cast<int32_t>(btn));
	if (state == GLFW_PRESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Fracture::Input::IsMouseUp(MouseCode btn)
{
	return false;
}

bool Fracture::Input::MousePressed(MouseCode btn)
{
	return false;
}

bool Fracture::Input::MouseReleased(MouseCode btn)
{
	return false;
}

bool Fracture::Input::IsMouseScrolling()
{
	return false;
}

glm::vec2 Fracture::Input::GetMousePosition()
{
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);
	if (glfwGetWindowAttrib(IFractureApp::mWindow->Context, GLFW_HOVERED))
	{
		double m_x, m_y;
		glfwGetCursorPos(IFractureApp::mWindow->Context, &m_x, &m_y);
		pos.x = (float)m_x;
		pos.y = (float)m_y;
	}
	return pos;
}

glm::vec2 Fracture::Input::GetMouseScroll()
{
	return m_scroll;
}

void Fracture::Input::RegisterKeyMap(const std::string& Name, KeyCode key)
{
	mKeyMapping[Name] = key;
}

void Fracture::Input::RemoveKeyMap(const std::string& Name)
{
}

bool Fracture::Input::ButtonPressed(const std::string& Name)
{
	int state = glfwGetKey(IFractureApp::mWindow->Context, static_cast<int32_t>(mKeyMapping[Name]));
	if (state == GLFW_PRESS && state != GLFW_REPEAT)
		return true;


	return false;
}

bool Fracture::Input::ButtonReleased(const std::string& Name)
{
	return false;
}

void Fracture::Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FRACTURE_TRACE("mouse scroll x:{} y:{}", xoffset, yoffset);

	m_scroll.x = (float)xoffset;
	m_scroll.y = (float)yoffset;
}

void Fracture::Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//TODO Fire off Key Events
}

void Fracture::Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//TODO Fire off mouse position Events
}

void Fracture::Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//TODO Fire off mouse Events
}
