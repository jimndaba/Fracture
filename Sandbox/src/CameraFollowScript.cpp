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
	
	glm::vec3 targetPosition = tagerTtransform->Position + offset;
	
	camera->Position = glm::lerp(camera->Position,targetPosition,smoothTime * dt);
	FRACTURE_TRACE("Camera Position: {} , {} ,{} ",camera->Position.x, camera->Position.y, camera->Position.z);
	camera->LookAt(tagerTtransform->Position);
}

void Fracture::CameraFollowScript::onLateUpdate(float dt)
{
}
