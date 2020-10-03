#include "Application.h"
#include "Editor/Editor.h"

Fracture::Application::Application()
{
	m_Editor = std::unique_ptr<Editor>(new Editor());
}

Fracture::Application::~Application()
{
}

void Fracture::Application::init()
{
	m_Editor->onInit();
}

void Fracture::Application::run()
{
	init();
	while (m_IsRunning)
	{
		m_Editor->onUpdate();
	}
}

void Fracture::Application::onUpdate()
{
}

void Fracture::Application::onDraw()
{
}
