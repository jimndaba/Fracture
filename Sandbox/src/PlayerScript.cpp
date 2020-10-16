#include "PlayerScript.h"

Fracture::PlayerScript::PlayerScript(uint32_t id):GameLogic(),m_id(id)
{
}

void Fracture::PlayerScript::onStart()
{
	transform = GetComponent<TransformComponent>(m_id);
}

void Fracture::PlayerScript::onUpdate(float dt)
{
	
	if (InputManager::IsKeyDown(KeyCode::W))
	{
		position.y += playerSpeed * dt;
	}
	if (InputManager::IsKeyDown(KeyCode::S))
	{
		position.y -= playerSpeed * dt;
	}
	if (InputManager::IsKeyDown(KeyCode::A))
	{
		position.x -= playerSpeed * dt;
	}
	if (InputManager::IsKeyDown(KeyCode::D))
	{
		position.x += playerSpeed * dt;
	}

	transform->Position += position;
	
}


void Fracture::PlayerScript::onLateUpdate(float dt)
{
}
