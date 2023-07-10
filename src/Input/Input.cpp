#include "FracturePCH.h"
#include "Common/Logger.h"
#include "Input.h"
#include "Common/AppWindow.h"
#include "Common/FractureApp.h"
#include "EventSystem/Eventbus.h"


glm::vec2 Fracture::Input::m_scroll;
std::map<std::string, Fracture::KeyCode> Fracture::Input::mKeyMapping;
std::unordered_map<std::string, Fracture::InputBinding> Fracture::Input::InputBindings;

Fracture::Input::Input()
{
	glfwSetScrollCallback(IFractureApp::mWindow->Context, Fracture::Input::scroll_callback);
	glfwSetCursorPosCallback(IFractureApp::mWindow->Context, cursor_position_callback);
	glfwSetMouseButtonCallback(IFractureApp::mWindow->Context, mouse_button_callback);
	glfwSetKeyCallback(IFractureApp::mWindow->Context, key_callback);
	glfwSetJoystickCallback(Joystick_callback);
	
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::Space;
		k->IsRepeating = 0;
		k->Multiplier = 1;
		BindInput("Shoot", k);
	}
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::B;
		k->IsRepeating = 0;
		k->Multiplier = 1;
		BindInput("Shoot", k);
	}
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::A;
		k->IsRepeating = 0;
		k->Multiplier = -1;
		BindInput("vertical", k, InputBinding::Type::Axis);
	}
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::D;
		k->IsRepeating = 0;
		k->Multiplier = 1;
		BindInput("vertical", k,InputBinding::Type::Axis);
	}
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::Left;
		k->IsRepeating = 0;
		k->Multiplier = -1;
		BindInput("vertical", k, InputBinding::Type::Axis);
	}
	{
		std::shared_ptr<InputSource> k = std::make_shared<InputSource>();
		k->Device = InputSource::SourceType::Keyboard;
		k->key = (uint8_t)KeyCode::Right;
		k->IsRepeating = 0;
		k->Multiplier = 1;
		BindInput("vertical", k, InputBinding::Type::Axis);
	}
	Eventbus::Subscribe(this, &Fracture::Input::OnKeyboardEvent);


}

void Fracture::Input::Update()
{
	/*
	for (const auto& gamepad : mGamePads)
	{


	}
	*/
}

bool Fracture::Input::IsKeyDown(KeyCode key)
{
	//return mKeyboardDevice->IsKeyPressed(key);

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
	return false;
}

bool Fracture::Input::IsMouseUp(MouseCode btn)
{
	return false;
}

bool Fracture::Input::MousePressed(MouseCode btn)
{
	int32_t state = glfwGetMouseButton(IFractureApp::mWindow->Context, static_cast<int32_t>(btn));
	if (state == GLFW_PRESS && state != GLFW_REPEAT)
	{
		return true;
	}
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

void Fracture::Input::OnKeyboardEvent(const std::shared_ptr<KeyboardEvent>& evnt)
{
	for (auto binding : InputBindings)
	{
		for (auto source : binding.second.Sources)
		{
			if (source->Device == Fracture::InputSource::SourceType::Keyboard && source->key == evnt->key)
			{
				if(source->InputState == InputSource::State::Released)
				{
					source->InputState = InputSource::State::Up;
					source->FLOAT = 0.0f;
				}
				if (evnt->action == GLFW_PRESS)
				{
					source->InputState = InputSource::State::Pressed;
					source->FLOAT = 1.0f;
				}
				if (evnt->action == GLFW_RELEASE)
				{
					source->InputState = InputSource::State::Released;
					source->FLOAT = 0.0f;
				}
			    if (evnt->action == GLFW_REPEAT)
				{
					source->InputState = InputSource::State::Down;
					source->FLOAT = 1.0f;
				}			
			}
			
		}
	}

}

void Fracture::Input::OnGamepadConnectedEvent(const std::shared_ptr<GamepadConnectEvent>& evnt)
{
	NoConnectedGamepads += 1;
}

void Fracture::Input::OnGamepadDisconnectedEvent(const std::shared_ptr<GamepadDisconnectedEvent>& evnt)
{
	NoConnectedGamepads -= 1;
}

void Fracture::Input::OnSaveBingdingsEvent(const std::shared_ptr<SaveBidingsEvent>& evnt)
{
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
	if (InputBindings.find(Name) == InputBindings.end())
		return false;
	
	if (InputBindings[Name].BindingType == Fracture::InputBinding::Type::Button)
	{
		for (auto source : InputBindings[Name].Sources)
		{			
			if (source->InputState == InputSource::State::Pressed)
			{
				switch (InputBindings[Name].BindingType)
				{
					case InputBinding::Type::Button:
					{
						source->FLOAT = 1.0f;
						break;
					}
					case InputBinding::Type::Axis:
					{
						source->FLOAT = 1.0f * source->Multiplier;
						break;
					}
				}
				
				return true;
			}
		};
	}
	return false;
}

bool Fracture::Input::ButtonReleased(const std::string& Name)
{
	if (InputBindings.find(Name) == InputBindings.end())
		return false;

	if (InputBindings[Name].BindingType == Fracture::InputBinding::Type::Button)
	{
		for (auto source : InputBindings[Name].Sources)
		{
			if (source->InputState == InputSource::State::Released)
			{
				switch (InputBindings[Name].BindingType)
				{
				case InputBinding::Type::Button:
				{
					source->FLOAT = 0.0f;
					break;
				}
				case InputBinding::Type::Axis:
				{
					source->FLOAT = 0.0f;
					break;
				}
				}
				source->InputState = InputSource::State::Up;
				return true;
			}
		};
	}
	return false;
}

float Fracture::Input::GetAxis(const std::string& Name)
{
	if (InputBindings.find(Name) == InputBindings.end())
		return 0.0f;

	if (InputBindings[Name].BindingType == Fracture::InputBinding::Type::Axis)
	{
		for (auto source : InputBindings[Name].Sources)
		{			
			if (source->Device == InputSource::SourceType::Keyboard && source->InputState == InputSource::State::Pressed)
			{
				return source->FLOAT * source->Multiplier;
			}
		};
	}
	return 0.0f;
}

void Fracture::Input::BindInput(const std::string& action)
{
	InputBindings[action] = InputBinding();
}

void Fracture::Input::BindInput(const std::string& action,const std::shared_ptr<InputSource>& source, InputBinding::Type bindingType)
{
	InputBindings[action].BindingType = bindingType;
	InputBindings[action].Sources.push_back(source);
}

void Fracture::Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FRACTURE_TRACE("mouse scroll x:{} y:{}", xoffset, yoffset);

	m_scroll.x = (float)xoffset;
	m_scroll.y = (float)yoffset;
}

void Fracture::Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Eventbus::Publish<KeyboardEvent>(key, scancode, action, mods);
}

void Fracture::Input::cursor_position_callback(GLFWwindow* window,double xpos, double ypos)
{
	Eventbus::Publish<MouseMoveEvent>(xpos,ypos);
}

void Fracture::Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//TODO Fire off Eventbus::Publish<KeyboardEvent>(key, scancode, action, mods);
}

void Fracture::Input::Joystick_callback(int jid, int event)
{
}
