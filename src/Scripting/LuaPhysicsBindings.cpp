#include "FracturePCH.h"
#include "LuaPhysicsBindings.h"
#include "Physics/PhysicsEvents.h"

void Fracture::BindPhysicsEvents(sol::state* lua)
{
	lua->new_usertype<Fracture::CollisionContext>("contactInfo",
		// Constructor 
		sol::constructors <
		Fracture::CollisionContext()
		>(),
		//Members
		"entity", &Fracture::CollisionContext::entity,
		"other", &Fracture::CollisionContext::other),
		//Metamethods
		sol::meta_function::to_string, [](Fracture::CollisionContext& e) { return fmt::format("contactInfo : Collsion:{},{}", e.entity, e.other);};
}
