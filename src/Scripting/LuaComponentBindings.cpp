#include "FracturePCH.h"
#include "LuaComponentBindings.h"
#include "World/Components.h"
#include "World/TransformSystem.h"
#include "World/PhysicsSystem.h"

void LuaBindComponents::BindTagComponent(sol::state& lua)
{
	lua.new_usertype<Fracture::TagComponent>("TagComponent",
		"TagComponent", sol::no_constructor,
		sol::base_classes, sol::bases<Fracture::IComponent>(),
		//Properties
		"Name", sol::readonly_property(&Fracture::TagComponent::Name),
		"ID", sol::readonly_property(&Fracture::TagComponent::GetID),


		//Metamethods
		sol::meta_function::to_string, [](Fracture::TagComponent& e) { return fmt::format("TagComponent : {}, {}", e.Name, e.GetID()); }
	);
}

void LuaBindComponents::BindTransformComponent(sol::state& lua)
{
	lua.new_usertype<Fracture::TransformComponent>("TransformComponent",
		"TransformComponent", sol::no_constructor,
		sol::base_classes, sol::bases<Fracture::IComponent>(),
		//Properties
		"Position", sol::readonly_property(&Fracture::TransformComponent::Position),
		"Rotation", sol::readonly_property(&Fracture::TransformComponent::Rotation),
		"Scale", sol::readonly_property(&Fracture::TransformComponent::Scale),
		"ID", sol::readonly_property(&Fracture::TransformComponent::GetID),

		//Methods
		"Translate", &Fracture::TransformSystem::Translate,		
		"Lookat", & Fracture::TransformSystem::LookAt,
		"SetPosition", &Fracture::TransformSystem::SetPosition,
		"SetRotation", &Fracture::TransformSystem::SetRotation,
		"SetScale", &Fracture::TransformSystem::SetScale,

		//Metamethods
		sol::meta_function::to_string, [](Fracture::TransformComponent& e) { return fmt::format("TransformComponent : {}, Pos:{},{},{}", e.GetID(), e.Position.x, e.Position.y, e.Position.z); }
	);
}

void LuaBindComponents::BindHierachyComponent(sol::state& lua)
{
}

void LuaBindComponents::BindMeshComponent(sol::state& lua)
{
}

void LuaBindComponents::BindColliderComponent(sol::state& lua)
{
	lua.new_usertype<Fracture::ColliderComponent>("ColliderComponent",
		"ColliderComponent", sol::no_constructor,
		sol::base_classes, sol::bases<Fracture::IComponent>(),
		//Properties
		"Height", sol::readonly_property(&Fracture::ColliderComponent::Height),
		"Size", sol::readonly_property(&Fracture::ColliderComponent::Size),
		"Radius", sol::readonly_property(&Fracture::ColliderComponent::Radius),
		"Offset", sol::readonly_property(&Fracture::ColliderComponent::Offset),
		"ID", sol::readonly_property(&Fracture::ColliderComponent::GetID),

		//Methods
	

		//Metamethods
		sol::meta_function::to_string, [](Fracture::ColliderComponent& e) { return fmt::format("ColliderComponent : {}", e.GetID()); }
	);
}

void LuaBindComponents::BindRigidbodyComponent(sol::state& lua)
{
	lua.new_usertype<Fracture::RigidbodyComponent>("RigibodyComponent",
		"RigibodyComponent", sol::no_constructor,
		sol::base_classes, sol::bases<Fracture::IComponent>(),
		//Properties
		"Mass", sol::readonly_property(&Fracture::RigidbodyComponent::Mass),
		"AngularVelocity", sol::readonly_property(&Fracture::RigidbodyComponent::AngularVelocity),
		"LinearVelocity", sol::readonly_property(&Fracture::RigidbodyComponent::LinearVelocity),
		"IsKinematic", sol::readonly_property(&Fracture::RigidbodyComponent::IsKinematic),
		"IsDynamic", sol::readonly_property(&Fracture::RigidbodyComponent::IsDynamic),
		"ID", sol::readonly_property(&Fracture::RigidbodyComponent::GetID),

		//Methods
		"AddForce", & Fracture::PhysicsSystem::AddForce,
		"AddTorque", & Fracture::PhysicsSystem::AddTorque,
		"SetLinearVelocity", & Fracture::PhysicsSystem::SetLinearVelocity,
		"SetLinearDrag", & Fracture::PhysicsSystem::SetLinearDrag,

		//Metamethods
		sol::meta_function::to_string, [](Fracture::RigidbodyComponent& e) { return fmt::format("RigidbodyComponent : {}", e.GetID()); }
	);

	
	std::initializer_list<std::pair<sol::string_view, Fracture::ForceMode>> keyItems =
	{
		{"Acceleration", Fracture::ForceMode::Acceleration},
		{"Force", Fracture::ForceMode::Force},
		{"Impulse", Fracture::ForceMode::Impulse},
		{"VelocityChange", Fracture::ForceMode::VelocityChange}
	};
	lua.new_enum<Fracture::ForceMode, false>("ForceMode", keyItems);
	
}

void LuaBindComponents::BindPointLightComponent(sol::state& lua)
{
}

void LuaBindComponents::BindSpotLightComponent(sol::state& lua)
{
}

void LuaBindComponents::BindSunLightComponent(sol::state& lua)
{
}

void LuaBindComponents::BindCameraComponent(sol::state& lua)
{
}

void LuaBindComponents::BindAnimatorComponent(sol::state& lua)
{
}
