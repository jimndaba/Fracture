#include "Game.h"
#include "GameWindow.h"
#include "Rendering/Renderer.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Scene/Scene.h"



std::shared_ptr<Fracture::Scene> test;
double t = 0.0;
const double dt = 0.01;

double currentTime = SDL_GetTicks() / 1000.0;
double accumulator = 0.0;


Fracture::Game::Game()
{
	m_GameWindow = std::unique_ptr<GameWindow>(new GameWindow(1280,720,"FRACTURE"));
	m_Renderer = std::unique_ptr<Renderer>(new Renderer(*m_GameWindow));
	m_ComponentManager = std::unique_ptr<ComponentManager>(new ComponentManager());
	m_AssetManager = Fracture::AssetManager::instance();
	m_EntityManager = std::unique_ptr<EntityManager>(new EntityManager());
	m_InputManager = std::unique_ptr<InputManager>(new InputManager());
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
		InputManager::PollEvents();

		double newTime = (double)SDL_GetTicks()/1000.0;
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		//while (frameTime > 0.0)
		//{
		//	float deltaTime = (float)fmin(frameTime, dt);
		//fixed step
		//	frameTime -= deltaTime;
		//	t += deltaTime;
		//}
		update(frameTime);
		render();
		m_GameWindow->swapBuffers();
	}
	unloadContent();
	shutdown();
}

void Fracture::Game::init()
{
	m_Renderer->clearColor(0.2f, 0.5f, 0.6f);
	test = std::make_shared<Scene>();
}

void Fracture::Game::loadContent()
{
	test->onLoad();
	


}

void Fracture::Game::update(float dt)
{
	m_ComponentManager->onUpdate();

	
	std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(1);

	transform->Rotation.x += 0.5f * dt;
	transform->Rotation.y += 0.8f * dt;

	if (InputManager::IsKeyDown(KeyCode::Escape))
	{
		onQuit();
	}

	m_Renderer->update(dt);
}

void Fracture::Game::render()
{
	m_Renderer->BeginFrame(test);
	m_Renderer->RenderPasses();
	m_Renderer->EndFrame();
}

void Fracture::Game::unloadContent()
{
	test->clearScene();
	test.reset();
}

void Fracture::Game::shutdown()
{
	
	m_AssetManager.release();
	m_Renderer.release();
	m_ComponentManager.release();
	m_EntityManager.release();
	m_GameWindow.release();
}

void Fracture::Game::onQuit()
{
	m_isRunning = false;
}

void Fracture::Game::onWindowResize(int width, int height)
{
	m_Renderer->setViewport(width, height);
}
