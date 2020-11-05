#include "IDManager.h"

uint32_t Fracture::IDManager::m_currentID;

Fracture::IDManager::IDManager()
{
}

Fracture::IDManager::~IDManager()
{
}

void Fracture::IDManager::ResetIDs()
{
	m_currentID = 1;
}

void Fracture::IDManager::NextID()
{
	m_currentID++;
}
