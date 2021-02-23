#include "EntityManager.h"
#include "UUID.h"

Fracture::EntityManager::EntityManager()
{
}

std::shared_ptr<Fracture::Entity> Fracture::EntityManager::CreateEntity(UUID id)
{
	return std::make_shared<Entity>(UUID(id));
}
