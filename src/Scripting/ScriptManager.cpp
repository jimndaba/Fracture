#include "FracturePCH.h"
#include "ScriptManager.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "Input/Input.h"
#include "LuaScript.h"

sol::state* Fracture::ScriptManager::lua;

Fracture::ScriptManager::ScriptManager()
{

}

void Fracture::ScriptManager::BindLog(sol::state& L)
{
	auto log = L.create_table("Debug");
	log.set_function("log", [&](sol::this_state s, std::string message) {
		FRACTURE_INFO(message);
		});

	log.set_function("trace", [&](sol::this_state s, std::string message) {
		FRACTURE_TRACE(message);
		});

	log.set_function("error", [&](sol::this_state s, std::string message) {
		FRACTURE_ERROR(message);
		});

	log.set_function("warn", [&](sol::this_state s, std::string message) {
		FRACTURE_WARN(message);
		});

	log.set_function("critical", [&](sol::this_state s, std::string message) {
		FRACTURE_CRITICAL(message);
		});
}
void Fracture::ScriptManager::BindCore(sol::state& lua)
{
	lua.new_usertype<Fracture::IComponent>("Component",
		sol::meta_function::to_string, [](Fracture::IComponent& e) { return fmt::format("Component : {}", e.GetID()); }
	);
}
void Fracture::ScriptManager::BindFunctions(sol::state& lua)
{
	//lua.set_function("GetEntity", [](sol::this_state s, std::string name)->Entity* {
	//	return ScriptManager::GetEntity(name);
	//	});

	lua.set_function("GetTagComponent", [](sol::this_state s, UUID id)->TagComponent* {
		return ScriptManager::GetComponentByType<TagComponent>(id);
		});
	lua.set_function("GetTransformComponent", [](sol::this_state s, UUID id)->TransformComponent* {
		return ScriptManager::GetComponentByType<TransformComponent>(id);
		});
	lua.set_function("GetCameraComponent", [](sol::this_state s, UUID id)->CameraComponent* {
		return ScriptManager::GetComponentByType<CameraComponent>(id);
		});
	lua.set_function("GetRigidBodyComponent", [](sol::this_state s, UUID id)->RigidbodyComponent* {
		return ScriptManager::GetComponentByType<RigidbodyComponent>(id);
		});
	lua.set_function("GetColliderComponent", [](sol::this_state s, UUID id)->ColliderComponent* {
		return ScriptManager::GetComponentByType<ColliderComponent>(id);
		});
	lua.set_function("Destroy", sol::overload([](sol::this_state s, UUID id) {
		return SceneManager::RemoveEntity(id);
		}));
	lua.set_function("Destroy", sol::overload([](sol::this_state s, Entity entity) {
		return SceneManager::RemoveEntity(entity.ID);
		}));
}
void Fracture::ScriptManager::BindInput(sol::state& L)
{
	auto input = L.create_table("Input");
	input.set_function("IsKeyPressed", [](Fracture::KeyCode key) -> bool {
		return Input::IsKeyDown(key);
		});

	std::initializer_list<std::pair<sol::string_view, Fracture::KeyCode>> keyItems =
	{
		{"A", Fracture::KeyCode::A},
		{"B", Fracture::KeyCode::B},
		{"C", Fracture::KeyCode::C},
		{"D", Fracture::KeyCode::D},
		{"E", Fracture::KeyCode::E},
		{"F", Fracture::KeyCode::F},
		{"H", Fracture::KeyCode::G},
		{"G", Fracture::KeyCode::H},
		{"I", Fracture::KeyCode::I},
		{"J", Fracture::KeyCode::J},
		{"K", Fracture::KeyCode::K},
		{"L", Fracture::KeyCode::L},
		{"M", Fracture::KeyCode::M},
		{"N", Fracture::KeyCode::N},
		{"O", Fracture::KeyCode::O},
		{"P", Fracture::KeyCode::P},
		{"Q", Fracture::KeyCode::Q},
		{"R", Fracture::KeyCode::R},
		{"S", Fracture::KeyCode::S},
		{"T", Fracture::KeyCode::T},
		{"U", Fracture::KeyCode::U},
		{"V", Fracture::KeyCode::V},
		{"W", Fracture::KeyCode::W},
		{"X", Fracture::KeyCode::X},
		{"Y", Fracture::KeyCode::Y},
		{"Z", Fracture::KeyCode::Z},
		{"Space", Fracture::KeyCode::Space},
		{"Escape", Fracture::KeyCode::Escape},
		{ "APOSTROPHE", Fracture::KeyCode::Apostrophe },
		{"Comma", Fracture::KeyCode::Comma},
		{ "MINUS", Fracture::KeyCode::Minus },
		{ "PERIOD", Fracture::KeyCode::Period },
		{ "SLASH", Fracture::KeyCode::Slash },
		{ "SEMICOLON", Fracture::KeyCode::Semicolon },
		{ "EQUAL", Fracture::KeyCode::Equal },
		{ "LEFT_BRACKET", Fracture::KeyCode::LeftBracket },
		{ "BACKSLASH", Fracture::KeyCode::Backslash },
		{ "RIGHT_BRACKET", Fracture::KeyCode::RightBracket },
		{"Enter", Fracture::KeyCode::Enter},
		{"Tab", Fracture::KeyCode::Tab},
		{"Backspace", Fracture::KeyCode::Backspace},
		{"Insert", Fracture::KeyCode::Insert},
		{"Delete", Fracture::KeyCode::Delete},
		{"Right", Fracture::KeyCode::Right},
		{"Left", Fracture::KeyCode::Left},
		{"Down", Fracture::KeyCode::Down},
		{"Up", Fracture::KeyCode::Up},
		{"PageUp", Fracture::KeyCode::PageUp},
		{"PageDown", Fracture::KeyCode::PageDown},
		{"Home", Fracture::KeyCode::Home},
		{"End", Fracture::KeyCode::End},
		{ "CAPS_LOCK", Fracture::KeyCode::CapsLock },
		{ "SCROLL_LOCK", Fracture::KeyCode::ScrollLock },
		{"NumLock", Fracture::KeyCode::NumLock},
		{"PrintScreen", Fracture::KeyCode::PrintScreen},
		{"Pasue", Fracture::KeyCode::Pause},
		{"LeftShift", Fracture::KeyCode::LeftShift},
		{"LeftControl", Fracture::KeyCode::LeftControl},
		{ "LEFT_ALT", Fracture::KeyCode::LeftAlt },
		{ "LEFT_SUPER", Fracture::KeyCode::LeftSuper },
		{"RightShift", Fracture::KeyCode::RightShift},
		{"RightControl", Fracture::KeyCode::RightControl},
		{ "RIGHT_ALT", Fracture::KeyCode::RightAlt },
		{ "RIGHT_SUPER", Fracture::KeyCode::RightSuper },
		{"Menu", Fracture::KeyCode::Menu},
		{"F1", Fracture::KeyCode::F1},
		{"F2", Fracture::KeyCode::F2},
		{"F3", Fracture::KeyCode::F3},
		{"F4", Fracture::KeyCode::F4},
		{"F5", Fracture::KeyCode::F5},
		{"F6", Fracture::KeyCode::F6},
		{"F7", Fracture::KeyCode::F7},
		{"F8", Fracture::KeyCode::F8},
		{"F9", Fracture::KeyCode::F9},
		{"F10", Fracture::KeyCode::F10},
		{"F11", Fracture::KeyCode::F11},
		{"F12", Fracture::KeyCode::F12},
		{"Keypad0", Fracture::KeyCode::D0},
		{"Keypad1", Fracture::KeyCode::D1},
		{"Keypad2", Fracture::KeyCode::D2},
		{"Keypad3", Fracture::KeyCode::D3},
		{"Keypad4", Fracture::KeyCode::D4},
		{"Keypad5", Fracture::KeyCode::D5},
		{"Keypad6", Fracture::KeyCode::D6},
		{"Keypad7", Fracture::KeyCode::D7},
		{"Keypad8", Fracture::KeyCode::D8},
		{"Keypad9", Fracture::KeyCode::D9},
		{"Decimal", Fracture::KeyCode::Period},
		{"Divide", Fracture::KeyCode::Slash},
		{"Multiply", Fracture::KeyCode::KPMultiply},
		{"Subtract", Fracture::KeyCode::Minus},
		{"Add", Fracture::KeyCode::KPAdd},
		{ "KP_EQUAL",    Fracture::KeyCode::KPEqual }
	};
	L.new_enum<Fracture::KeyCode, false>("key", keyItems);
}
void Fracture::ScriptManager::BindMaths(sol::state& L)
{
	//LuaBindGLM::BindVec2(L);
	//LuaBindGLM::BindVec3(L);
	//LuaBindGLM::BindVec4(L);
}
void Fracture::ScriptManager::BindApplication(sol::state& L)
{

}

void Fracture::ScriptManager::Init()
{
	lua = new(sol::state);
	lua->open_libraries(sol::lib::base);
	BindCore(*lua);
	BindLog(*lua);
	BindInput(*lua);
	BindFunctions(*lua);
	BindMaths(*lua);

	//LuaBindEntity::BindEntity(*lua);
	//LuaBindComponents::BindTagComponent(*lua);
	//LuaBindComponents::BindAnimatorComponent(*lua);
	//LuaBindComponents::BindCameraComponent(*lua);
	//LuaBindComponents::BindColliderComponent(*lua);
	//LuaBindComponents::BindLightComponent(*lua);
	//LuaBindComponents::BindRelationShipComponent(*lua);
	//LuaBindComponents::BindRenderComponent(*lua);
	//LuaBindComponents::BindRigidBodyComponent(*lua);
	//LuaBindComponents::BindTagComponent(*lua);
	//LuaBindComponents::BindTransformComponent(*lua);
	FRACTURE_INFO("Script Manager Init");

}
void Fracture::ScriptManager::onStart()
{

}
void Fracture::ScriptManager::onExit()
{

}
void Fracture::ScriptManager::onUpdate(float dt)
{

}

void Fracture::ScriptManager::Shutdown()
{
	delete(lua);
}

void Fracture::ScriptManager::Reload(LuaScript* mscript)
{
	FRACTURE_INFO("Script {} Reloaded", mscript->Description.Name);
	mscript->Reload(*lua);
}

void Fracture::ScriptManager::LoadScript(const std::shared_ptr<LuaScript>& mscript)
{
	mscript->Load(*lua);
}

sol::state* Fracture::ScriptManager::GetState()
{
	return lua;
}


Fracture::Entity* Fracture::ScriptManager::GetEntity(const std::string& name)
{
	for (auto& tag : SceneManager::GetAllComponents<TagComponent>())
	{
		if (tag->Name == name)
		{
			//return SceneManager::CurrentScene()->Entities. (tag->ID);
		}
	}
	FRACTURE_ERROR("Cannot Find Entity: {}", name);
	return nullptr;
}