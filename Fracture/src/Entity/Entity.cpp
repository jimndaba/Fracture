#include "Entity.h"

Fracture::Entity::Entity(int id):Id(id)

{
}

Fracture::Entity::~Entity()
{
}

void Fracture::Entity::addChild(std::shared_ptr<Entity> child)
{
	child->setParent((std::shared_ptr<Entity>)this);
	m_children.push_back(child);
}

void Fracture::Entity::removeChild(std::shared_ptr<Entity> child)
{
	std::vector<std::shared_ptr<Entity>>::iterator new_end;
	new_end = remove(m_children.begin(), m_children.begin(), child);
}

void Fracture::Entity::clearChildren()
{
	m_children.clear();
}

void Fracture::Entity::setParent(std::shared_ptr<Entity> parent)
{
	Parent = parent;
}

std::vector<std::shared_ptr<Fracture::Entity>> Fracture::Entity::Children()
{
	return m_children;
}
