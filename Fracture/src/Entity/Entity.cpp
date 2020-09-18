#include "Entity.h"

#include <iostream>

Fracture::Entity::Entity(int id):Id(id)
{

}

Fracture::Entity::~Entity()
{
	std::cout << "destroy entity: "<< name << std::endl;
	Parent = nullptr;	
}

void Fracture::Entity::addChild(std::shared_ptr<Entity> child)
{
	child->setParent((std::shared_ptr<Entity>)this);
	m_children.push_back(child);
}

void Fracture::Entity::removeChild(std::shared_ptr<Entity> child)
{
	//std::vector<Entity>::iterator new_end;
	//new_end = std::remove(m_children.begin(),m_children.end(), child);
}

void Fracture::Entity::clearChildren()
{
	std::cout << "clear children" << std::endl;	
	for (int i =0;i <m_children.size();i++)
	{
		m_children[i].reset();
	}
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
