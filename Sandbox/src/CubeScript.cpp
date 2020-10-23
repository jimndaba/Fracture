#include "CubeScript.h"

Fracture::CubeScript::CubeScript(int id):GameLogic(),m_id(id)
{

}


void Fracture::CubeScript::onStart()
{
	transform = ComponentManager::GetComponent<TransformComponent>(m_id);

}

void Fracture::CubeScript::onUpdate(float dt)
{

	transform->Rotation.x += 0.5f * dt * m_id;
	transform->Rotation.y += 2.0f * dt * m_id;

}

void Fracture::CubeScript::onLateUpdate()
{

}

void Fracture::CubeScript::onCollision(CollisionEvent* collision)
{
}
