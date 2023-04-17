#include "FracturePCH.h"
#include "AudioManager.h"

Fracture::AudioManager::AudioManager()
{
}

void Fracture::AudioManager::OnInit()
{
	gSoloud.init();
}

void Fracture::AudioManager::OnLoadContent()
{	
}

void Fracture::AudioManager::OnUpdate(float dt)
{
}

void Fracture::AudioManager::OnShutdown()
{
	gSoloud.deinit(); // Clean up!
}
