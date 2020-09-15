#include "Game.h"
#include "GameWindow.h"
#include "Rendering/Renderer.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Entity/Entity.h"
#include "Scene/Scene.h"


std::shared_ptr<Fracture::Scene> test;



Fracture::Game::Game()
{
	m_GameWindow = std::unique_ptr<GameWindow>(new GameWindow());
	m_Renderer = std::unique_ptr<Renderer>(new Renderer());
	m_ComponentManager = std::unique_ptr<ComponentManager>(new ComponentManager());
	m_AssetManager = std::unique_ptr<AssetManager>(new AssetManager());
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
	m_Renderer->clearColor(0.2f, 0.5f, 0.6f);
	test = std::shared_ptr<Scene>(new Scene());
}

void Fracture::Game::loadContent()
{
	m_AssetManager->AddModel("monkey","bin/content/models/monkey.fbx");
	m_AssetManager->AddShader("default","bin/content/shaders/model/vertex.glsl","bin/content/shaders/model/fragment.glsl");
	m_AssetManager->AddMaterial("default",m_AssetManager->getShader("default"));
	
	std::shared_ptr<Entity> monkey = std::shared_ptr<Entity>(new Entity(2));
	ComponentManager::AddComponent<TransformComponent>(monkey->Id, glm::vec3(0.0f, 0.0f, 0.0f));
	ComponentManager::AddComponent<RenderComponent>(monkey->Id, "monkey", "default");
	test->addEntity(monkey);


}

void Fracture::Game::update()
{
	m_ComponentManager->onUpdate();
}

void Fracture::Game::render()
{
	m_Renderer->BeginFrame(test);
	m_Renderer->RenderPasses();
	m_Renderer->EndFrame();
}

void Fracture::Game::unloadContent()
{


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
