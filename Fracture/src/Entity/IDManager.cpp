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
