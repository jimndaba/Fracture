#include "GameLogic.h"
#include "Entity/Entity.h"
#include "Component/ComponentManager.h"
#include "Scene/SceneManager.h"

void Fracture::GameLogic::destroy(uint32_t entityID)
{
	ComponentManager::RemoveComponentsbyID(entityID);	
	SceneManager::RemoveEntity(entityID);
}

void Fracture::GameLogic::destroy(std::shared_ptr<Entity> entity)
{	
	ComponentManager::RemoveComponentsbyID(entity->Id);
	SceneManager::RemoveEntity(entity);
}
