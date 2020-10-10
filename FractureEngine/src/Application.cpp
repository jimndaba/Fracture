#include "Application.h"
#include "Editor/Editor.h"
#include "SandboxScene.h"


Fracture::Application::Application()
{
	m_Editor = std::unique_ptr<Editor>(new Editor());	
	m_state = EngineState::Game;
}

Fracture::Application::~Application()
{

}

void Fracture::Application::init()
{
	m_Editor->onInit();
	m_Game = std::unique_ptr<Game>(new Game(1280, 720));	
	m_Game->init();

	m_Game->addScene(std::make_shared<SandboxScene>());

	m_Game->loadContent();
	m_Editor->SetGame(m_Game.get());
}

void Fracture::Application::run()
{
	init();	
	while (!m_Editor->done)
	{
		onUpdate();
		onDraw();
	}
	m_Game.release();
}

void Fracture::Application::onUpdate()
{	
	if (m_state == EngineState::Game)
	{
		m_Game->update(1/60.0f);
	}

}

void Fracture::Application::onDraw()
{
	if (m_state == EngineState::Game)
	{
		m_Game->render();
	}
	m_Editor->onUpdate();
}
