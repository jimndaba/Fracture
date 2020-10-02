#include "Game.h"
#include "GameWindow.h"
#include "Rendering/Renderer.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Entity/Entity.h"
#include "Entity/Camera.h"
#include "Component/CameraControllerComponent.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Entity/IDManager.h"
#include "Scene/Scene.h"



std::shared_ptr<Fracture::Scene> test;
double t = 0.0;
const double dt = 0.01;
double currentTime = SDL_GetTicks() / 1000.0;
double accumulator = 0.0;


Fracture::Game::Game()
{
	m_GameWindow = std::unique_ptr<GameWindow>(new GameWindow(1280,720,"FRACTURE"));
	m_AssetManager = Fracture::AssetManager::instance();
	m_ComponentManager = std::unique_ptr<ComponentManager>(new ComponentManager());
	m_Renderer = std::unique_ptr<Renderer>(new Renderer(*m_GameWindow));		
	m_EntityManager = std::unique_ptr<EntityManager>(new EntityManager());
	m_InputManager = std::unique_ptr<InputManager>(new InputManager());
	m_IDManager = std::unique_ptr<IDManager>(new IDManager());
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
	test = std::make_shared<Scene>();
}

void Fracture::Game::loadContent()
{
	test->onLoad();
}

void Fracture::Game::update(float dt)
{
	m_ComponentManager->onUpdate(dt);

	std::shared_ptr<CameraControllerComponent> camera = ComponentManager::GetComponent<CameraControllerComponent>(test->MainCamera()->Id);
	
	for (int i = 3; i < 13; i++)
	{
		std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(i);
		transform->Rotation.x += 0.5f * dt * i;
		transform->Rotation.y += 0.8f * dt * i;
	}

	float mouseX = m_InputManager->GetMousePosition().x;
	float mouseY = m_InputManager->GetMousePosition().y;
	

	if (InputManager::IsMouseDown(MOUSECODE::ButtonRight))
	{
		camera->InputMouse(mouseX, mouseY, dt);
		if (InputManager::IsKeyDown(KeyCode::W))
		{
			camera->Move(Camera_Movement::FORWARD, dt);
		}
		if (InputManager::IsKeyDown(KeyCode::S))
		{
			camera->Move(Camera_Movement::BACKWARD, dt);
		}
		if (InputManager::IsKeyDown(KeyCode::A))
		{
			camera->Move(Camera_Movement::LEFT, dt);
		}
		if (InputManager::IsKeyDown(KeyCode::D))
		{
			camera->Move(Camera_Movement::RIGHT, dt);
		}
		if (InputManager::IsKeyDown(KeyCode::E))
		{
			camera->Move(Camera_Movement::UP, dt);
		}
		if (InputManager::IsKeyDown(KeyCode::Q))
		{
			camera->Move(Camera_Movement::DOWN, dt);
		}		
	}

	
	if (InputManager::IsKeyDown(KeyCode::Escape))
	{
		onQuit();
	}	
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
