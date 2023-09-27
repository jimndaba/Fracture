#include "FracturePCH.h"
#include "FractureGame.h"
#include "Rendering/GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "Rendering/PostProcessPipeline.h"
#include "Physics/PhysicsManager.h"
#include "Audio/AudioManager.h"
#include "Input/Input.h"
#include "Rendering/DebugRenderer.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"
#include "Rendering/SceneRenderer.h"
#include "World/TransformSystem.h"
#include "World/CameraSystem.h"
#include "Animation/AnimationSystem.h"

int _GamefpsCount = 0;
static float GamelastTime = 0.0f;
float currentGameTime = glfwGetTime();
bool Fracture::FractureGame::GameRunning;

Fracture::FractureGame::FractureGame()
{
    
}

bool Fracture::FractureGame::Run(AppWindow* window,SceneRenderer* renderer)
{
    GameRunning = true;    
   
    if (SceneManager::CurrentScene())
    {
        Fracture::TransformSystem tSystem{};
        tSystem.Update(SceneManager::CurrentScene()->RootID);

        const auto& entity = SceneManager::GetComponent<HierachyComponent>(SceneManager::CurrentScene()->RootID);
        for (const auto& prefab : entity->Prefabs)
        {
            tSystem.UpdatePrefabs(prefab);
        }
    }

    PhysicsManager::Instance()->CreateScene();
    PhysicsManager::Instance()->AddActors();
    PhysicsManager::Instance()->AddCharacterControllers();

    //TODO On Start Scripts
   
    mScriptManager->onStart();
    
    return GameRunning;
}

bool Fracture::FractureGame::Startup(AppWindow* window)
{
    GameRunning = true;
    return true;
}

bool Fracture::FractureGame::CreateGameWindow(AppWindow* window)
{
    WindowCreationInfo window_info;
    window_info.Height = 1920;
    window_info.Width = 1080;
    window_info.Name = "Game";
    window_info.IsMaximised = true;
    window_info.IsFullscreen = false;
    if(window)
        window_info.mSharedContext = window->Context;

    mWindow = CreateAppWindow(&window_info);
    if (!mWindow)
    {
        FRACTURE_ERROR("Failed to Init App Window!");
        return false;
    }

    return true;
}

void Fracture::FractureGame::OnLoadContent()
{   
}

void Fracture::FractureGame::Update()
{
   
    //mWindow->PollEvents();

 
    float newTime = glfwGetTime();
    float frameTime = newTime - currentGameTime;
    currentGameTime = newTime;    
        

    mScriptManager->onUpdate(frameTime);

    

    mScriptManager->onLateUpdate(frameTime);

    Fracture::CameraSystem mCameraSystem{};
    for (const auto& camera : SceneManager::GetAllComponents<CameraComponent>())
    {
        mCameraSystem.Update(frameTime, *camera.get());
    }

    //Systems Update
    if (SceneManager::CurrentScene())
    {
        Fracture::TransformSystem tSystem{};
        tSystem.Update(SceneManager::CurrentScene()->RootID);

        const auto& entity = SceneManager::GetComponent<HierachyComponent>(SceneManager::CurrentScene()->RootID);
        for (const auto& prefab : entity->Prefabs)
        {
            tSystem.UpdatePrefabs(prefab);
        }
    }
 

    AnimationSystem::Instance()->Update(frameTime);

    mAudioManager->OnUpdate(frameTime);
    
}

void Fracture::FractureGame::OnFrameStart(SceneRenderer* renderer)
{
    OPTICK_EVENT();
    GraphicsDevice::Instance()->ClearBuffers((uint32_t)ClearBufferBit::Color);
    Fracture::GraphicsDevice::Instance()->ClearColor(0.1f, 0.6f, 0.9f, 1.0f);
    GraphicsDevice::Instance()->DRAWCALL_COUNT = 0;

    renderer->Begin(0.0f);
    renderer->End();
}

void Fracture::FractureGame::OnDebugDraw()
{
    PhysicsManager::Instance()->OnDebugDraw();
}

void Fracture::FractureGame::FixedUpdate(float dt)
{
    OPTICK_EVENT();
    mScriptManager->onFixedUpdate();
    PhysicsManager::Instance()->FixedUpdate(1.0f / 60.0f);
}

void Fracture::FractureGame::Shutdown()
{      
    mScriptManager->onExit();

    PhysicsManager::Instance()->RemoveActors();

    PhysicsManager::Instance()->DestroyScene();    
}
