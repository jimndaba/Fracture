#include "FracturePCH.h"
#include "PhysicsEvents.h"
#include "World/SceneManager.h"

Fracture::OnCollisionEvent::OnCollisionEvent(CollisionContext c) :Event(),Collision(c)
{
}

Fracture::OnAddActorEvent::OnAddActorEvent(SceneManager* sm, UUID entity):Event(),manager(sm),id(entity)
{
}
