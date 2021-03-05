#include "ScriptManager.h"
#include "Logging/Logger.h"
#include "LuaScript.h"
#include "Input/InputManager.h"

Fracture::ScriptManager::ScriptManager()
{

	lua.open_libraries(sol::lib::base);

	BindLog(lua);
	BindInput(lua);

	script = LuaScript::Create("CameraController", "content/scripts/");
	script->Load(lua);

	script2 = LuaScript::Create("PlayerController", "content/scripts/");
	script2->Load(lua);
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

void Fracture::ScriptManager::BindInput(sol::state& L)
{
	auto input = L.create_table("Input");

	input.set_function("IsKeyPressed", [](Fracture::KeyCode key) -> bool {
		return InputManager::IsKeyDown(key);
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
}

void Fracture::ScriptManager::BindApplication(sol::state& L)
{
}

void Fracture::ScriptManager::onStart()
{
	script->onStart(lua);
}

void Fracture::ScriptManager::onExit()
{
	script->onExit(lua);
}

void Fracture::ScriptManager::onUpdate(float dt)
{
	script->Reload(lua);
	script->onUpdate(lua,dt);

	script2->Reload(lua);
	script2->onUpdate(lua, dt);
}

void Fracture::ScriptManager::Reload(std::shared_ptr<LuaScript> mscript)
{
	mscript->Reload(lua);
	mscript->BindFunction(lua);
}
