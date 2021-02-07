#include "Entity.h"
#include "IDManager.h"
#include <iostream>
#include "Component/RelationshipComponent.h"
#include "Component/ComponentManager.h"
#include "Logging/Logger.h"

Fracture::Entity::Entity()
{
}

Fracture::Entity::Entity(uint32_t id):Id(id)
{

}

Fracture::Entity::~Entity()
{
	
}

