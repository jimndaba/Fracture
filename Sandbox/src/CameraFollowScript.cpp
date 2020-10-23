#include "CameraFollowScript.h"
#include "Logging/Logger.h"

Fracture::CameraFollowScript::CameraFollowScript(uint32_t id):GameLogic(),m_id(id)
{
}

void Fracture::CameraFollowScript::onStart()
{
	 camera = ComponentManager::GetComponent<CameraControllerComponent>(m_id);
	 tagerTtransform = ComponentManager::GetComponent<TransformComponent>(target);

}

void Fracture::CameraFollowScript::onUpdate(float dt)
{	
	targetPosition = tagerTtransform->Position + offset; // 
	camera->LookAt(tagerTtransform->Position);
	camera->Position = glm::lerp(camera->Position, targetPosition, dt * smoothTime);


}

void Fracture::CameraFollowScript::onLateUpdate(float dt)
{
}

void Fracture::CameraFollowScript::onCollision(CollisionEvent* collision)
{
}
