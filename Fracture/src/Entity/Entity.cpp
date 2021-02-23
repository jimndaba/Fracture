#include "Entity.h"
#include "IDManager.h"
#include <iostream>
#include "Component/RelationshipComponent.h"
#include "Component/ComponentManager.h"
#include "Logging/Logger.h"

Fracture::Entity::Entity():
	m_ID(UUID())
{
	std::cout << m_ID << std::endl;
}

Fracture::Entity::Entity(UUID id):
	m_ID(id)
{
}


Fracture::Entity::~Entity()
{
	
}

Fracture::UUID Fracture::Entity::GetId()
{
	return m_ID;
}

