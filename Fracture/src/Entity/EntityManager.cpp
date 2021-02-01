#include "EntityManager.h"

Fracture::EntityManager::EntityManager()
{
}

std::shared_ptr<Fracture::Entity> Fracture::EntityManager::CreateEntity(uint32_t id)
{
	return std::make_shared<Entity>(id);
}
