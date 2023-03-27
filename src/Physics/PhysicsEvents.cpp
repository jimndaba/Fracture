#include "FracturePCH.h"
#include "PhysicsEvents.h"

Fracture::OnCollisionEvent::OnCollisionEvent(CollisionContext a, CollisionContext b) :Event(),EntityA(a),EntityB(b)
{
}
