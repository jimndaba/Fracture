#include "DirectLight.h"
#include "Entity/Entity.h"
#include "Component/ComponentManager.h"
#include "Component/DirectLightComponent.h"

Fracture::DirectLight::DirectLight():Entity()
{
	ComponentManager::AddComponent<DirectLightComponent>(this->Id);
}

Fracture::DirectLight::~DirectLight()
{
}
