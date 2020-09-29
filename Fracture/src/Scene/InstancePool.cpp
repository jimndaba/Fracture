#include "InstancePool.h"

Fracture::InstancePool::InstancePool(std::string name):Name(name)
{
}

Fracture::InstancePool::~InstancePool()
{
}

void Fracture::InstancePool::AddInstance(std::shared_ptr<EntityInstance> instance)
{
	m_instances.push_back(instance);
}

void Fracture::InstancePool::Clear()
{
	m_instances.clear();
}

std::vector<std::shared_ptr<Fracture::EntityInstance>> Fracture::InstancePool::GetInstances()
{
	return m_instances;
}
