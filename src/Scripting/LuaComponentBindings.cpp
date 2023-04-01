#include "FracturePCH.h"
#include "LuaComponentBindings.h"
#include "World/Components.h"

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
		"position", sol::readonly_property(&Fracture::TransformComponent::Position),
		"rotation", sol::readonly_property(&Fracture::TransformComponent::Rotation),
		"scale", sol::readonly_property(&Fracture::TransformComponent::Scale),
		"ID", sol::readonly_property(&Fracture::TransformComponent::GetID),

		//Methods
		//"translate", &Fracture::TransformComponent::Translate,
		//"setPosition", &Fracture::TransformComponent::SetPosition,
		//"setRotation", &Fracture::TransformComponent::SetRotation,
		//"setScale", &Fracture::TransformComponent::SetScale,
		//"lookat", & Fracture::TransformComponent::LookAt,

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
		"height", sol::readonly_property(&Fracture::ColliderComponent::Height),
		"size", sol::readonly_property(&Fracture::ColliderComponent::Size),
		"radius", sol::readonly_property(&Fracture::ColliderComponent::Radius),
		"offset", sol::readonly_property(&Fracture::ColliderComponent::Offset),
		"ID", sol::readonly_property(&Fracture::ColliderComponent::GetID),

		//Methods
		//"addForce", &Fracture::ColliderComponent::setAddForce,
		//"addTorque", &Fracture::ColliderComponent::AddTorque,
		//"setLinearVelocity", &Fracture::ColliderComponent::SetLinearVelocity,
		//"setLinearDrag", &Fracture::ColliderComponent::SetLinearDrag,

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
		"mass", sol::readonly_property(&Fracture::RigidbodyComponent::Mass),
		//"angularVelocity", sol::readonly_property(&Fracture::RigidbodyComponent::GetAngularVelocity),
		//"LinearVelocity", sol::readonly_property(&Fracture::RigidbodyComponent::GetLinearVelocity),
		"isKinematic", sol::readonly_property(&Fracture::RigidbodyComponent::IsKinematic),
		"isDynamic", sol::readonly_property(&Fracture::RigidbodyComponent::IsDynamic),
		"ID", sol::readonly_property(&Fracture::RigidbodyComponent::GetID),

		//Methods
		//"addForce", &Fracture::RigidbodyComponent::AddForce,
		//"addTorque", &Fracture::RigidbodyComponent::AddTorque,
		//"setLinearVelocity", &Fracture::RigidbodyComponent::SetLinearVelocity,
		//"setLinearDrag", &Fracture::RigidbodyComponent::SetLinearDrag,

		//Metamethods
		sol::meta_function::to_string, [](Fracture::RigidbodyComponent& e) { return fmt::format("RigidbodyComponent : {}", e.GetID()); }
	);

	/*
	std::initializer_list<std::pair<sol::string_view, Fracture::ForceMode>> keyItems =
	{
		{"Acceleration", Fracture::ForceMode::Acceleration},
		{"Force", Fracture::ForceMode::Force},
		{"Impulse", Fracture::ForceMode::Impulse},
		{"VelocityChange", Fracture::ForceMode::VelocityChange}
	};
	lua.new_enum<Fracture::ForceMode, false>("forceMode", keyItems);
	*/
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
