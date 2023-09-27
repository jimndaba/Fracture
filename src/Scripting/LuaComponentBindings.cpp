#include "FracturePCH.h"
#include "LuaComponentBindings.h"
#include "World/Components.h"
#include "World/TransformSystem.h"
#include "World/PhysicsSystem.h"
#include "World/SceneManager.h"
#include "EventSystem/Eventbus.h"
#include "World/WorldEvents.h"
#include "Physics/PhysicsManager.h"
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
		"Position", &Fracture::TransformComponent::Position,
		"Rotation", &Fracture::TransformComponent::Rotation,
		"Scale", &Fracture::TransformComponent::Scale,
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


glm::vec3 LuaBindComponents::GetPosition(Fracture::UUID entity)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (transform)
		return transform->Position;
	return glm::vec3();
}

glm::vec3 LuaBindComponents::GetScale(Fracture::UUID entity)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (transform)
		return transform->Scale;
	return glm::vec3();
}

glm::vec3 LuaBindComponents::GetRotation(Fracture::UUID entity)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (transform)
		return glm::eulerAngles(transform->Rotation);
	return glm::vec3();
}

void LuaBindComponents::SetPosition(Fracture::UUID entity, glm::vec3 Position)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;

	transform->Position = Position;
}

void LuaBindComponents::SetRotation(Fracture::UUID entity, glm::vec3 value)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;

	transform->Rotation *= glm::quat(value);
}

void LuaBindComponents::SetScale(Fracture::UUID entity, glm::vec3 value)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;
	transform->Scale = value;
}

void LuaBindComponents::Translate(Fracture::UUID entity, glm::vec3 value)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;

	transform->Position += value;
}

void LuaBindComponents::Rotate(Fracture::UUID entity, glm::vec3 value)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;

	transform->Rotation = glm::normalize(transform->Rotation * glm::quat(value));
}

void LuaBindComponents::LookAt(Fracture::UUID entity, glm::vec3 value, glm::vec3 up)
{
	const auto& transform = Fracture::SceneManager::GetComponent<Fracture::TransformComponent>(entity);
	if (!transform)
		return;

	Fracture::TransformSystem::LookAt(transform, value, up);
}

void LuaBindComponents::Instantiate(Fracture::UUID entity, glm::vec3 position, glm::vec3 rotation)
{
	Fracture::Eventbus::Publish<Fracture::InstantiatePrefabEvent>(entity, position,rotation);
}

glm::vec3 LuaBindComponents::Vec3Lerp(glm::vec3 from, glm::vec3 to, float t)
{
	return glm::lerp(from, to, t);
}

glm::vec2 LuaBindComponents::Vec2Lerp(glm::vec2 from, glm::vec2 to, float t)
{
	return glm::lerp(from, to, t);
}

glm::vec4 LuaBindComponents::Vec4Lerp(glm::vec4 from, glm::vec4 to, float t)
{
	return glm::lerp(from, to, t);
}

