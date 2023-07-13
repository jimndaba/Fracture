#include "FracturePCH.h"
#include "SceneManager.h"
#include "Serialisation/SceneSerialiser.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/LuaScript.h"
#include "World/TransformSystem.h"
#include "EventSystem/Eventbus.h"
#include "Physics/PhysicsEvents.h"

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::mCurrentScene;
std::map<Fracture::UUID, Fracture::SceneRegistry> Fracture::SceneManager::mSceneRegister;
std::map<std::string, Fracture::UUID> Fracture::SceneManager::mSceneIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Scene>> Fracture::SceneManager::mScenes;
std::shared_ptr<Fracture::CameraComponent> Fracture::SceneManager::mActiveCamera;
std::unordered_map<Fracture::UUID, std::vector<Fracture::UUID>> Fracture::SceneManager::mScript_Entities;


Fracture::SceneManager::SceneManager()
{
    mCurrentScene = nullptr;
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::CreateNewScene(const UUID& root)
{
    auto id = UUID();
    mScenes[id] = std::make_shared<Scene>();
    mScenes[id]->Name = "Untitled";
    mScenes[id]->ID = id;
    mScenes[id]->RootID = root;   
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

void Fracture::SceneManager::RemovePrefab(const UUID& entity)
{
    if (!mCurrentScene)
        return;

    auto it = std::find_if(std::begin(mCurrentScene->mPrefabs), std::end(mCurrentScene->mPrefabs), [&](ScenePrefab p) { return p.PrefabID == entity; });

    if (it != mCurrentScene->mPrefabs.end())
    {
        int index = std::distance(mCurrentScene->mPrefabs.begin(), it);

        FRACTURE_TRACE("Destroying Prefab: {}", mCurrentScene->mPrefabs[index].PrefabID);


        mCurrentScene->mPrefabs.erase(
            std::remove(std::begin(mCurrentScene->mPrefabs),
                std::end(mCurrentScene->mPrefabs), mCurrentScene->mPrefabs[index]),
            std::end(mCurrentScene->mPrefabs));
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

void Fracture::SceneManager::SetActiveCamera(const Fracture::UUID& camera_id)
{
    if (HasComponent<CameraComponent>(camera_id))
    {
        mCurrentScene->ActiveCameraID = camera_id;
        mActiveCamera = GetComponent<CameraComponent>(mCurrentScene->ActiveCameraID);
    }
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

void Fracture::SceneManager::InstanceSceneFromFile(ScenePrefab prefab)
{
    auto it = mSceneRegister.find(prefab.SceneID);
    if (it != mSceneRegister.end())
    {
        SceneSerialiser loader(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
        loader.Open(mSceneRegister[prefab.SceneID].Path);
        loader.ReadScenePrefab(prefab);

        for (const auto& r : loader.MeshesToLoad)
            Eventbus::Publish<AsyncLoadMeshEvent>(r.first);


    }
}

std::map<Fracture::UUID,int> Fracture::SceneManager::LoadSceneByID(const UUID& scene_ID)
{    
    auto it = mSceneRegister.find(scene_ID);
    if (it != mSceneRegister.end())
    {
        SceneSerialiser loader(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
        loader.Open(mSceneRegister[scene_ID].Path);
        auto scene = loader.ReadScene();

        if (scene)
        {
            scene->ID = scene_ID;
            mScenes[scene_ID] = scene;
            mCurrentScene = mScenes[scene_ID];
      
            //for (const auto& prefab : mCurrentScene->mPrefabs)
            //{
            //    Instantiate(prefab);
           // }
        }


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

    {
        mScenes[scene_ID]->mScriptReg.clear();
        mScenes[scene_ID]->ComponentReg.clear();
        mScenes[scene_ID]->Entities.clear();
        mScenes[scene_ID]->mPrefabs.clear();
        mScenes[scene_ID].reset();
        mScenes.erase(scene_ID);
    }
}

void Fracture::SceneManager::SetScene(const std::shared_ptr<Scene>& scene)
{
    //if (mCurrentScene)
   // {
   // /    UnloadSceneByID(mCurrentScene->ID);
   //     mCurrentScene.reset();
   // }

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

Fracture::UUID Fracture::SceneManager::Instantiate(UUID prefab, glm::vec3 position, glm::vec3 scale, glm::quat rotation)
{

    ScenePrefab mPrefab;
    mPrefab.PrefabID = UUID();
    mPrefab.ParentID = mCurrentScene->RootID;
    mPrefab.SceneID = prefab;
    mPrefab.Position = position;
    mPrefab.Rotation = rotation;
    mPrefab.Scale = scale;

    InstanceSceneFromFile(mPrefab);
    mCurrentScene->mPrefabs.push_back(mPrefab);
    return mPrefab.PrefabID;
}

Fracture::UUID Fracture::SceneManager::InstantiateAsChildOf(UUID prefab, UUID parent, glm::vec3 position)
{
    ScenePrefab mPrefab;
    mPrefab.PrefabID = UUID();
    mPrefab.ParentID = parent;
    mPrefab.SceneID = prefab;
    mPrefab.Position = position;
    mPrefab.Rotation = glm::quat();
    InstanceSceneFromFile(mPrefab);
    mCurrentScene->mPrefabs.push_back(mPrefab);
    return mPrefab.PrefabID;
}

bool Fracture::SceneManager::IsTaggedWith(const UUID& id, const std::string& tag)
{
    const auto& component = GetComponent<TagComponent>(id);

    auto it = std::find_if(component->Tags.begin(), component->Tags.end(),
        [tag](const std::string& m) { return m == tag; });

    if (it != component->Tags.end())
        return true;

    return false;
}

Fracture::UUID Fracture::SceneManager::GetEntityWithTag(const std::string& tag)
{
    const auto& tagcomponents = GetAllComponents<TagComponent>();
    for (const auto& component : tagcomponents)
    {
        for (const auto& c_tag : component->Tags)
        {
            if (c_tag == tag)
                return component->GetID();
        }
    }
    return 0;
}

bool Fracture::SceneManager::IsPrefabScene(const UUID& id)
{
    if (mCurrentScene)
    {
       auto it = std::find_if(mCurrentScene->mPrefabs.begin(), mCurrentScene->mPrefabs.end(),
            [id](const ScenePrefab& m){ return m.PrefabID == id; });
      
       if (it != mCurrentScene->mPrefabs.end())
           return true;    
    }

    return false;
}

Fracture::ScenePrefab Fracture::SceneManager::GetScenePrefab(const UUID& id)
{
    if (mCurrentScene)
    {
        auto it = std::find_if(mCurrentScene->mPrefabs.begin(), mCurrentScene->mPrefabs.end(),
            [id](const ScenePrefab& m) { return m.PrefabID == id; });

        if (it != mCurrentScene->mPrefabs.end())
            return (ScenePrefab)*it;
    }

    return ScenePrefab();

}

Fracture::UUID Fracture::SceneManager::Instantiate(ScenePrefab prefab)
{
    InstanceSceneFromFile(prefab);
    mCurrentScene->mPrefabs.push_back(prefab);
    return prefab.PrefabID;
}

void Fracture::SceneManager::InstanceComponents(UUID prefab, UUID new_entity, UUID original_entity, UUID parent_entity)
{
    InstanceComponent<TagComponent>(prefab, new_entity, original_entity);
    InstanceComponent<TransformComponent>(prefab, new_entity, original_entity);
    InstanceComponent<HierachyComponent>(prefab, new_entity, original_entity);
    InstanceComponent<MeshComponent>(prefab, new_entity, original_entity);
    InstanceComponent<ColliderComponent>(prefab, new_entity, original_entity);
    InstanceComponent<RigidbodyComponent>(prefab, new_entity, original_entity);
    InstanceComponent<PointlightComponent>(prefab, new_entity, original_entity);
    InstanceComponent<SpotlightComponent>(prefab, new_entity, original_entity);
    InstanceComponent<SunlightComponent>(prefab, new_entity, original_entity);
 

    /*
    const auto& new_hierachy = GetComponent<HierachyComponent>(new_entity);
    if (new_hierachy)
    {
        new_hierachy->HasParent = true;
        new_hierachy->Parent = parent_entity;
    }

    const auto& hierachy = GetInstanceComponent<HierachyComponent>(prefab,original_entity);
    for (const auto& child : hierachy->Children)
    {
        UUID childentity = UUID();       
        new_hierachy->Children.push_back(childentity);
        InstanceComponents(prefab, childentity, child,new_entity);
    }
    */
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<TagComponent>& component, UUID new_entity)
{
    AddComponentByInstance<TagComponent>(new_entity,component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<HierachyComponent>& component, UUID new_entity)
{
    AddComponent<HierachyComponent>(new_entity);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<TransformComponent>& component, UUID new_entity)
{
    AddComponent<TransformComponent>(new_entity);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<MeshComponent>& component, UUID new_entity)
{
    AddComponentByInstance<MeshComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<PointlightComponent>& component, UUID new_entity)
{
    AddComponentByInstance<PointlightComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<SpotlightComponent>& component, UUID new_entity)
{
    AddComponentByInstance<SpotlightComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<SunlightComponent>& component, UUID new_entity)
{
    AddComponentByInstance<SunlightComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<ColliderComponent>& component, UUID new_entity)
{
    AddComponentByInstance<ColliderComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<RigidbodyComponent>& component, UUID new_entity)
{
    AddComponentByInstance<RigidbodyComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<AudioSourceComponent>& component, UUID new_entity)
{
    AddComponentByInstance<AudioSourceComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<ScriptComponent>& component, UUID new_entity)
{
    AddComponentByInstance<ScriptComponent>(new_entity, component);
}

void Fracture::SceneManager::AddComponentInstance(std::shared_ptr<CameraComponent>& component, UUID new_entity)
{
    AddComponentByInstance<CameraComponent>(new_entity, component);
}


