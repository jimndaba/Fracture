#include "Game.h"
#include "Logging/Logger.h"
#include "GameWindow.h"
#include "Serialisation/ProjectProperties.h"
#include "Rendering/Renderer.h"
#include "Serialisation/GameSettings.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Entity/EntityManager.h"
#include "Input/InputManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Profiling/Profiler.h"
#include "Physics/PhysicsManager.h"
//#include "Rendering/DebugRenderer.h"
#include "Rendering/Framegraph/FrameGraph.h"
#include "GameFramegraph.h"
#include "Event/Event.h"
#include "Event/Eventbus.h"
#include "Serialisation/GameSerializer.h"
#include "Serialisation/ProjectSerializer.h"
#include "Serialisation/SceneSerializer.h"
#include "Serialisation/FrameGraphSerialiser.h"

#include <iostream>

double t = 0.0;
const double dt = 0.01;
double currentTime = glfwGetTime() / 1000.0;
double accumulator = 0.0;

//std::unique_ptr<Fracture::ScriptManager> Fracture::Game::m_ScriptManager =0;
std::shared_ptr<Fracture::Eventbus> Fracture::Game::m_Eventbus=0;
std::shared_ptr<Fracture::Scene> Fracture::Game::m_currentScene=0;
std::unique_ptr<Fracture::SceneManager> Fracture::Game::m_SceneManager = 0;

Fracture::Game::Game()
{	
	std::cout << " NEW GAME" << std::endl;
	m_logger = std::make_unique<Logger>();
	m_Eventbus = std::make_shared<Eventbus>();
	m_GameSettings = std::make_shared<GameSettings>();
}

Fracture::Game::~Game()
{
}

void Fracture::Game::run()
{
	std::cout << " NEW GAME" << std::endl;
	Profiler::Get().BeginSession("Profile");	
	FRACTURE_TRACE("Engine Run");
	//init();

	loadContent();
	while (!m_GameWindow->ShouldClose() && m_isRunning)
	{
		m_GameWindow->pollEvents();
	
		double newTime = (double)glfwGetTime() /1000.0;
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
		
		while (accumulator >= dt)
		{
			update((float)dt);
			accumulator -= dt;
			t += dt;
			//m_ScriptManager->onEndFrame();
		}
	
		render();
	
		m_GameWindow->swapBuffers();
	}
	unloadContent();
	shutdown();
}

void Fracture::Game::init()
{	
	ProfilerTimer timer("Init");
	FRACTURE_TRACE("Engine Init");
	FRACTURE_TRACE("Load Game settings");
	GameSerializer loader = GameSerializer(m_GameSettings);
	loader.DeSerialize("Game.config");

	m_properties = std::make_shared<ProjectProperties>();


	FRACTURE_TRACE("Load Properties");
	ProjectSerializer seriliazer(m_properties);
	if (!seriliazer.DeSerializeProperties("CubeWars.Fracture"))
	{
		FRACTURE_ERROR("FAiLED to load Properties");
		return;
	}

	FRACTURE_INFO("Create Window");
	m_GameWindow = std::unique_ptr<GameWindow>(new GameWindow(
		m_GameSettings->Resolution_Width, 
		m_GameSettings->Resolution_Height,
		m_GameSettings->Title,
		m_GameSettings->IsResizable));

	
	m_ComponentManager = std::unique_ptr<ComponentManager>(new ComponentManager());
	m_ComponentManager->onInit();

	m_Renderer = std::shared_ptr<Renderer>(new Renderer());
	m_EntityManager = std::unique_ptr<EntityManager>(new EntityManager());
	m_InputManager = std::unique_ptr<InputManager>(new InputManager());	
	m_PhysicsManager = std::unique_ptr<PhysicsManager>(new PhysicsManager());
	m_SceneManager = std::make_unique<SceneManager>();
	m_AssetManager = std::unique_ptr<AssetManager>(new AssetManager(m_properties));
	m_framegraph = std::make_shared<GameFrameGraph>(*m_Renderer.get());
	//m_ScriptManager = std::unique_ptr<ScriptManager>(new ScriptManager());
	//m_debug = std::make_unique<DebugRenderer>();

	FRACTURE_INFO("Init Renderer");
	m_Renderer->onInit();
	m_Renderer->clearColor(1.0f, 0.0f, 0.0f);
	FRACTURE_INFO("Init Physcis");
	m_PhysicsManager->Init();
}

void Fracture::Game::loadContent()
{
	FRACTURE_TRACE("Engine LoadContent");
	ProfilerTimer timer("loadContent");
	ProjectSerializer seriliazer(m_properties);
	if (!seriliazer.DeSerialize("CubeWars.Fracture"))
	{
		FRACTURE_ERROR("FAiLED to load Project");
		return;
	}

	m_SceneManager->SetScene(m_properties->ActiveScene);

	{
		FRACTURE_TRACE("Load Framegraph");
		//Deserialise Scene Graph and build graph
		FrameGraphSerialiser graphSerialiser = FrameGraphSerialiser(m_framegraph, *m_Renderer.get());
		graphSerialiser.DeSerialiseGraph(m_properties->ScenesPath + "/" + m_properties->ActiveScene + ".graph");
		m_framegraph->Buildgraph();
	}

	if (m_SceneManager->GetActiveScene())
	{
		//m_SceneManager->GetActiveScene()->onLoad();
		m_ComponentManager->onLoad();
	}
}

void Fracture::Game::update(float dt)
{
	ProfilerTimer timer("Update");
	m_ComponentManager->onUpdate(dt);

	//if (m_ScriptManager->Start)
	//{
	//	m_ScriptManager->start();
	//	m_ScriptManager->Start = false;
	//}
	//m_ScriptManager->update(dt);

	m_PhysicsManager->onUpdate(dt);
		
	//std::shared_ptr<CameraControllerComponent> camera = ComponentManager::GetComponent<CameraControllerComponent>(m_currentScene->ActiveCamera()->GetId());

	float mouseX = m_InputManager->GetMousePosition().x;
	float mouseY = m_InputManager->GetMousePosition().y;

	/*
	if (InputManager::IsMouseDown(MOUSECODE::ButtonRight))
	{
		//SDL_WarpMouseInWindow(m_GameWindow->Context(), m_GameWindow->Width / 2, m_GameWindow->Height / 2);
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
	*/

	if (InputManager::IsKeyDown(KeyCode::Escape))
	{
		onQuit();
	}	
}

void Fracture::Game::render()
{
	ProfilerTimer timer("Render");
	m_Renderer->BeginFrame(m_SceneManager->GetActiveScene());

	m_framegraph->execute(*m_Renderer);

	m_Renderer->EndFrame();
}

void Fracture::Game::unloadContent()
{
	m_SceneManager->GetActiveScene()->clearScene();
}

void Fracture::Game::shutdown()
{
	Profiler::Get().EndSession();
	m_AssetManager.release();
	m_Renderer.reset();
	m_ComponentManager.release();
	m_EntityManager.release();
	m_GameWindow.release();
}

void Fracture::Game::addScene(std::string name,std::shared_ptr<Fracture::Scene> scene)
{
	m_SceneManager->AddScene(name, scene);
}

void Fracture::Game::addEntity(std::shared_ptr<Fracture::Entity> entity)
{
	m_SceneManager->GetActiveScene()->addEntity(entity);
}

void Fracture::Game::removeScene(std::string name)
{
	m_SceneManager->RemoveScene(name);
}

void Fracture::Game::changeScene(std::string name)
{
	m_SceneManager->SetScene(name);
}

std::shared_ptr<Fracture::Scene> Fracture::Game::CurrentScene()
{
	if(m_SceneManager->GetActiveScene())
		return m_SceneManager->GetActiveScene();
	return nullptr;
}

std::shared_ptr<Fracture::Eventbus> Fracture::Game::GetEventbus()
{
	return m_Eventbus;
}

void Fracture::Game::AddScript(std::shared_ptr<GameLogic> script)
{
	//m_ScriptManager->AddScript(script);
}

void Fracture::Game::onEvent(Event* mEvent)
{
	m_Eventbus->Publish(mEvent);
}

void Fracture::Game::onQuit()
{
	m_isRunning = false;
}

void Fracture::Game::onWindowResize(int width, int height)
{
	m_Renderer->setViewport(width, height);
}
