#include "Game.h"
#include "GameWindow.h"
#include "Rendering/Renderer.h"

#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Scene/Scene.h"


Fracture::Scene* test;

Fracture::Game::Game()
{
	m_GameWindow = std::shared_ptr<GameWindow>(new GameWindow());
	m_Renderer = std::unique_ptr<Renderer>(new Renderer());
	m_ComponentManager = std::unique_ptr<ComponentManager>(new ComponentManager());
}

Fracture::Game::~Game()
{
}

void Fracture::Game::run()
{
	init();
	loadContent();
	while (m_isRunning)
	{
		m_GameWindow->pollEvents(*this);
		update();
		render();
		m_GameWindow->swapBuffers();
	}
	unloadContent();
	shutdown();
}

void Fracture::Game::init()
{

	test = new Scene();
}

void Fracture::Game::loadContent()
{
	std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(1);

}

void Fracture::Game::update()
{
}

void Fracture::Game::render()
{
	m_Renderer->clear();
	m_Renderer->clearColor(0.2f, 0.5f, 0.6f);
}

void Fracture::Game::unloadContent()
{
	delete test;
}

void Fracture::Game::shutdown()
{
}

void Fracture::Game::onQuit()
{
	m_isRunning = false;
}

void Fracture::Game::onWindowResize(int width, int height)
{
	m_Renderer->setViewport(width, height);
}
