#include "FracturePCH.h"
#include "SceneManager.h"

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::mCurrentScene;

Fracture::SceneManager::SceneManager()
{
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::CreateNewScene(const UUID& root)
{
    auto new_scene = std::make_shared<Scene>();
    return  new_scene;
}

Fracture::UUID& Fracture::SceneManager::AddEntity()
{
    auto entity = std::make_shared<Entity>();
    auto& id = entity->ID;
  
    mCurrentScene->Entities.push_back(std::move(entity));
    return id;
}

void Fracture::SceneManager::AddEntity(const UUID& id)
{
    auto entity = std::make_shared<Entity>();
    entity->ID = id;
    mCurrentScene->Entities.push_back(std::move(entity));
}

void Fracture::SceneManager::RemoveEntity(const UUID& entity)
{
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

void Fracture::SceneManager::AttachScript()
{
}

void Fracture::SceneManager::DettachScript()
{
}

Fracture::Scene* Fracture::SceneManager::CurrentScene()
{
    return mCurrentScene.get();
}

void Fracture::SceneManager::SetScene(const std::shared_ptr<Scene>& scene)
{
    mCurrentScene = scene;
}

void Fracture::SceneManager::SetSceneByName(const std::string& scene)
{
}

void Fracture::SceneManager::SetSceneByID(const UUID& scene)
{
}

void Fracture::SceneManager::RegisterScene(const SceneRegistry& reg)
{
}

std::shared_ptr<Fracture::Scene> Fracture::SceneManager::LoadSceneFromFile(const std::string& path)
{
    return std::shared_ptr<Scene>();
}

void Fracture::SceneManager::SaveSceneToFile(const std::string& path)
{
}
