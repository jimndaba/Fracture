#include "FracturePCH.h"
#include "SceneManager.h"
#include "Serialisation/SceneSerialiser.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::mCurrentScene;
std::map<Fracture::UUID, Fracture::SceneRegistry> Fracture::SceneManager::mSceneRegister;
std::map<std::string, Fracture::UUID> Fracture::SceneManager::mSceneIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Scene>> Fracture::SceneManager::mScenes;
std::shared_ptr<Fracture::CameraComponent> Fracture::SceneManager::mActiveCamera;
std::unordered_map<Fracture::UUID, std::vector<Fracture::UUID>> Fracture::SceneManager::mScript_Entities;


Fracture::SceneManager::SceneManager()
{
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::CreateNewScene(const UUID& root)
{
    auto id = UUID();
    mScenes[id] = std::make_shared<Scene>();
    mScenes[id]->Name = "Untitled";
    mScenes[id]->ID = id;
    return  mScenes[id];
}

Fracture::UUID Fracture::SceneManager::AddEntity()
{
    if (!mCurrentScene)
        return UUID();

    auto entity = std::make_shared<Entity>();
    auto& id = entity->ID;
  
    mCurrentScene->Entities.push_back(std::move(entity));
    return id;
}

void Fracture::SceneManager::AddEntity(const UUID& id)
{
    if (!mCurrentScene)
        return;


    auto entity = std::make_shared<Entity>();
    entity->ID = id;
   
    mCurrentScene->Entities.push_back(std::move(entity));
}

void Fracture::SceneManager::RemoveEntity(const UUID& entity)
{
    if (!mCurrentScene)
        return;

    auto it = std::find_if(std::begin(mCurrentScene->Entities), std::end(mCurrentScene->Entities), [&](std::shared_ptr<Entity> p) { return p->ID == entity; });
    
    if (it != mCurrentScene->Entities.end())
    {
        int index = std::distance(mCurrentScene->Entities.begin(), it);

        FRACTURE_TRACE("Destroying Entity: {}", mCurrentScene->Entities[index]->ID);
     

        mCurrentScene->Entities.erase(
            std::remove(std::begin(mCurrentScene->Entities),
                std::end(mCurrentScene->Entities), mCurrentScene->Entities[index]),
            std::end(mCurrentScene->Entities));
    }
}

void Fracture::SceneManager::AttachScript(const UUID& entity_id)
{
    auto scriptcomp = std::make_shared<ScriptComponent>(entity_id);
    scriptcomp->HasScriptAttached = false;
    mCurrentScene->mScriptReg[entity_id].push_back(std::move(scriptcomp));
}

void Fracture::SceneManager::AttachScript(const UUID& entity_id, const UUID& script_id)
{
    auto scriptcomp = std::make_shared<ScriptComponent>(entity_id);
    scriptcomp->HasScriptAttached = true;
    scriptcomp->Script = script_id;  
    mCurrentScene->mScriptReg[entity_id].push_back(std::move(scriptcomp));
   
    mScript_Entities[script_id].push_back(entity_id);
}

void Fracture::SceneManager::DettachScript(const UUID& entity_id, const UUID& script_id)
{
    if (mCurrentScene->mScriptReg.find(entity_id) == mCurrentScene->mScriptReg.end())
        return;

    auto& script_array = mCurrentScene->mScriptReg[entity_id];

    const auto& it = std::find_if(std::begin(script_array), std::end(script_array), [&](std::shared_ptr<ScriptComponent> p) { return p->Script == script_id; });

    if (it != script_array.end())
    {
        int index = std::distance(script_array.begin(), it);

        FRACTURE_TRACE("Dettaching Script: {}", script_id);

        script_array.erase(
            std::remove(std::begin(script_array),
                std::end(script_array), script_array[index]),
            std::end(script_array));
    }
}

std::shared_ptr<Fracture::ScriptComponent> Fracture::SceneManager::GetScript(const UUID& entity_id, const UUID& script_id)
{
    if(mCurrentScene->mScriptReg.find(entity_id) == mCurrentScene->mScriptReg.end())
    return nullptr;
        
    const auto& script_array = mCurrentScene->mScriptReg[entity_id];

    auto it = std::find_if(std::begin(script_array), std::end(script_array), [&](std::shared_ptr<ScriptComponent> p) { return p->Script == script_id; });

    if (it != script_array.end())
    {
        return std::static_pointer_cast<ScriptComponent>(*it);
    }

    FRACTURE_ERROR("No Scipt {} found");
        
}

std::vector<std::shared_ptr<Fracture::ScriptComponent>> Fracture::SceneManager::GetAllEntityScripts(const UUID& entity_id)
{
    if (mCurrentScene->mScriptReg.find(entity_id) == mCurrentScene->mScriptReg.end())
        return std::vector<std::shared_ptr<Fracture::ScriptComponent>>();

    return mCurrentScene->mScriptReg[entity_id];
}

bool Fracture::SceneManager::HasScripts(const UUID& entity_id)
{
    if (mCurrentScene->mScriptReg.find(entity_id) == mCurrentScene->mScriptReg.end())
        return false;

    return mCurrentScene->mScriptReg[entity_id].size();
}

Fracture::Scene* Fracture::SceneManager::CurrentScene()
{
    return mCurrentScene.get();
}

std::shared_ptr<Fracture::CameraComponent> Fracture::SceneManager::ActiveCamera()
{
    if (!mActiveCamera)
        return nullptr;

    return mActiveCamera;
}

void Fracture::SceneManager::SetActiveCamera(const std::shared_ptr<CameraComponent>& mcamera)
{
    mActiveCamera = mcamera;
}

void Fracture::SceneManager::LoadScene(const std::string& name)
{
    auto scene_ID = mSceneIDLookUp[name];

    if (mScenes[scene_ID])
        return;
    LoadSceneByID(scene_ID);
}

Fracture::UUID& Fracture::SceneManager::LoadSceneFromFile(const std::string& path)
{   
    SceneSerialiser loader(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
    loader.Open(path);
    auto scene = loader.ReadScene();
    mScenes[scene->ID] = scene;

    auto it = mSceneRegister.find(scene->ID);
    if (it == mSceneRegister.end())
    {
        SceneRegistry reg;
        reg.ID = scene->ID;
        reg.Name = scene->Name;
        reg.Path = path;
        RegisterScene(reg);
    }

    return scene->ID;
}

std::map<Fracture::UUID,int> Fracture::SceneManager::LoadSceneByID(const UUID& scene_ID)
{    
    auto it = mSceneRegister.find(scene_ID);
    if (it != mSceneRegister.end())
    {
        SceneSerialiser loader(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
        loader.Open(mSceneRegister[scene_ID].Path);
        auto scene = loader.ReadScene();
        scene->ID = scene_ID;
        mScenes[scene_ID] = scene;
        mCurrentScene = mScenes[scene_ID];
        return loader.MeshesToLoad; 
    }
    return std::map<Fracture::UUID, int>();
}

void Fracture::SceneManager::UnloadScene(const std::string& name)
{
    auto scene_ID = mSceneIDLookUp[name];

    if (!mScenes[scene_ID])
        return;
  
    UnloadSceneByID(scene_ID);
}

void Fracture::SceneManager::UnloadSceneByID(const UUID& scene_ID)
{
    auto it = mSceneRegister.find(scene_ID);
    if (it != mSceneRegister.end())
    {
        mScenes[scene_ID]->mScriptReg.clear();
        mScenes[scene_ID]->ComponentReg.clear();
        mScenes[scene_ID]->Entities.clear();
        mScenes.erase(scene_ID);
    }
}

void Fracture::SceneManager::SetScene(const std::shared_ptr<Scene>& scene)
{
    if (mCurrentScene)
    {
        UnloadSceneByID(mCurrentScene->ID);
        mCurrentScene.reset();
    }

    mCurrentScene = scene;
}

void Fracture::SceneManager::SetSceneByName(const std::string& name)
{
    auto scene_ID = mSceneIDLookUp[name];
    SetSceneByID(scene_ID);
}

std::map<Fracture::UUID, int> Fracture::SceneManager::SetSceneByID(const UUID& scene_ID)
{
    if (mCurrentScene)
    {
        UnloadSceneByID(mCurrentScene->ID);
        mCurrentScene.reset();
    }

    auto it = mSceneRegister.find(scene_ID);    
    if (it != mSceneRegister.end())
    {

        if (!mScenes[scene_ID])
        {
            return LoadSceneByID(scene_ID);
        }       
    }

    FRACTURE_ERROR("Could not load Scene: {}", scene_ID);
    return std::map<UUID, int>();
}

void Fracture::SceneManager::RegisterScene(const SceneRegistry& reg)
{
    if (!HasScenePath(reg.Path))
    {
       mSceneRegister[reg.ID] = reg;
       mSceneIDLookUp[reg.Name] = reg.ID;
    }
}

bool Fracture::SceneManager::HasScenePath(const std::string& path)
{
    for (const auto& reg : mSceneRegister)
    {
        if (reg.second.Path == path)
            return true;
    }
    return false;
}


