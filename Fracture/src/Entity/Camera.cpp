#include "Camera.h"
#include "Entity.h"
#include "IDManager.h"
#include "Component/ComponentManager.h"


Fracture::Camera::Camera():Entity()
{
    ComponentManager::AddComponent<CameraControllerComponent>(this->Id);
}
