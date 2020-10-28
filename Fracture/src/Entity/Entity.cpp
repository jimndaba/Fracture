#include "Entity.h"
#include "IDManager.h"
#include <iostream>
#include "Component/RelationshipComponent.h"
#include "Component/ComponentManager.h"

Fracture::Entity::Entity():Id(IDManager::GetID())
{

}

Fracture::Entity::Entity(uint32_t id):Id(id)
{
}

Fracture::Entity::~Entity()
{
	
}

