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

int _GamefpsCount = 0;
static float GamelastTime = 0.0f;
float currentGameTime = glfwGetTime();
bool Fracture::FractureGame::GameRunning;

Fracture::FractureGame::FractureGame()
{
    
}

bool Fracture::FractureGame::Run(UUID entry_point, AppWindow* window,SceneRenderer* renderer)
{
    GameRunning = true;
    
    auto resources = SceneManager::SetSceneByID(entry_point);
    
    for (const auto& r : resources)
        Dispatcher->Publish<AsyncLoadMeshEvent>(r.first);

    for (const auto& s : SceneManager::mScript_Entities)
    {
        if (mScriptManager->mScripts.find(s.first) == mScriptManager->mScripts.end())
        {
            auto script = mScriptManager->GetInstanceOfScript(s.first);
            script->DoScript(*mScriptManager->GetState());
            script->BindProperties(*mScriptManager->GetState());
            mScriptManager->mScripts[s.first] = script;
        }
    }

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

    //TODO On Start Scripts
    mScriptManager->onStart();

    const auto& mCamera = SceneManager::GetComponent<CameraComponent>(SceneManager::CurrentScene()->ActiveCameraID);
    if (mCamera)
    {
        SceneManager::SetActiveCamera(mCamera);
    }
   
    //Update();

    //OnFrameStart(renderer);

    //Shutdown();
    return GameRunning;
}

bool Fracture::FractureGame::Startup(AppWindow* window)
{
    GameRunning = true;

    /*
    //logger = std::make_unique<Logger>();
    if(!CreateGameWindow(window))
    {
        return false;
    }

    Dispatcher = std::make_unique<Fracture::Eventbus>();

    InputManager = std::make_unique<Input>(mWindow.get());

    mDebugRenderer = std::make_unique<DebugRenderer>();
    mDebugRenderer->OnInit();

    sceneManger = std::make_unique<Fracture::SceneManager>();

    mScriptManager = std::make_unique<ScriptManager>();
    mScriptManager->Init();

    //mAudioManager = std::make_unique<AudioManager>();
    //mAudioManager->OnInit();

    //mAudioManager->OnLoadContent();
    */
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

    PhysicsManager::Instance()->FixedUpdate(1.0f / 60.0f);

    mAudioManager->OnUpdate(frameTime);

     
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

    Fracture::CameraSystem mCameraSystem{};
    for (const auto& camera : SceneManager::GetAllComponents<CameraComponent>())
    {
        mCameraSystem.Update(frameTime, *camera.get());
    }        

        //On DebugDraw
        //OnDebugDraw();        

       

        /*
        _fpsCount++;
        FrameCount++;

        if (frameTime > 1.0)
        {
            lastTime = newTime;
            FramesPerSec = _fpsCount;
            _fpsCount = 0;
            FrameCount = 0;
        }
        */

    
}

void Fracture::FractureGame::OnFrameStart(SceneRenderer* renderer)
{
    GraphicsDevice::Instance()->ClearBuffers((uint32_t)ClearBufferBit::Color);
    GraphicsDevice::Instance()->ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    GraphicsDevice::Instance()->DRAWCALL_COUNT = 0;

    renderer->Begin();

    renderer->End();
}

void Fracture::FractureGame::Shutdown()
{      
    mScriptManager->onExit();

    PhysicsManager::Instance()->RemoveActors();

    PhysicsManager::Instance()->DestroyScene();    
}
