#include "FracturePCH.h"
#include "ScriptManager.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "Input/Input.h"
#include "LuaScript.h"

#include "LuaMathBindings.h"
#include "LuaComponentBindings.h"

sol::state* Fracture::ScriptManager::lua;
std::map<Fracture::UUID, Fracture::LuaScriptRegistry> Fracture::ScriptManager::mScriptRegister;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::LuaScript>> Fracture::ScriptManager::mScripts;

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

	lua.new_usertype<Fracture::UUID>("UUID",
		// Constructor 
		sol::constructors <
		UUID()>(),

		sol::meta_function::to_string, [](Fracture::UUID& e) { return fmt::format("UUID : {}", e); }
	);

	//Constructors
	lua.set_function("UUID", []() {return UUID(); });
}

void Fracture::ScriptManager::BindFunctions(sol::state& lua)
{
	//lua.set_function("GetEntity", [](sol::this_state s, std::string name)->Entity* {
	//	return ScriptManager::GetEntity(name);
	//	});

	lua.set_function("GetTagComponent", [](sol::this_state s, UUID id)->std::shared_ptr<TagComponent> {
		return ScriptManager::GetComponentByType<TagComponent>(id);
		});
	lua.set_function("GetTransformComponent", [](sol::this_state s, UUID id)->std::shared_ptr<TransformComponent> {
		return ScriptManager::GetComponentByType<TransformComponent>(id);
		});
	lua.set_function("GetCameraComponent", [](sol::this_state s, UUID id)->std::shared_ptr<CameraComponent> {
		return ScriptManager::GetComponentByType<CameraComponent>(id);
		});
	lua.set_function("GetRigidBodyComponent", [](sol::this_state s, UUID id)->std::shared_ptr<RigidbodyComponent> {
		return ScriptManager::GetComponentByType<RigidbodyComponent>(id);
		});
	lua.set_function("GetColliderComponent", [](sol::this_state s, UUID id)->std::shared_ptr<ColliderComponent> {
		return ScriptManager::GetComponentByType<ColliderComponent>(id);
		});
	lua.set_function("GetSpolightComponent", [](sol::this_state s, UUID id)->std::shared_ptr<SpotlightComponent> {
		return ScriptManager::GetComponentByType<SpotlightComponent>(id);
		});
	lua.set_function("GetPointlightComponent", [](sol::this_state s, UUID id)->std::shared_ptr<PointlightComponent> {
		return ScriptManager::GetComponentByType<PointlightComponent>(id);
		});
	lua.set_function("GetSunlightComponent", [](sol::this_state s, UUID id)->std::shared_ptr<SunlightComponent> {
		return ScriptManager::GetComponentByType<SunlightComponent>(id);
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

	input.set_function("GetMousePosition", []() -> bool {
		return Input::GetMousePosition;
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
	LuaBindGLM::BindVec2(L);
	LuaBindGLM::BindVec3(L);
	LuaBindGLM::BindVec4(L);
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
	LuaBindComponents::BindTagComponent(*lua);
	LuaBindComponents::BindAnimatorComponent(*lua);
	LuaBindComponents::BindCameraComponent(*lua);
	LuaBindComponents::BindColliderComponent(*lua);
	LuaBindComponents::BindPointLightComponent(*lua);
	LuaBindComponents::BindSpotLightComponent(*lua);
	LuaBindComponents::BindSunLightComponent(*lua);
	LuaBindComponents::BindHierachyComponent(*lua);
	LuaBindComponents::BindMeshComponent(*lua);
	LuaBindComponents::BindRigidbodyComponent(*lua);
	LuaBindComponents::BindTagComponent(*lua);
	LuaBindComponents::BindTransformComponent(*lua);
	FRACTURE_INFO("Script Manager Init");

}

void Fracture::ScriptManager::onStart()
{
	for (const auto& entity : SceneManager::CurrentScene()->Entities)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity->ID);
		for (const auto& component : s)
		{
			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			if (!component->HasScriptAttached)
				continue;

			const auto& script = mScripts[component->Script];
			script->Load(*lua);
			script->BindFunctions(*lua);
			script->BindProperties(*lua);
			script->OnStart(*lua, component->GetID());
		}
	}
}

void Fracture::ScriptManager::onExit()
{
	for (const auto& entity : SceneManager::CurrentScene()->Entities)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity->ID);
		for (const auto& component : s)
		{
			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			if (!component->HasScriptAttached)
				continue;

			const auto& script = mScripts[component->Script];
			script->OnExit(*lua, component->GetID());
		}
	}
}

void Fracture::ScriptManager::onUpdate(float dt)
{
	for (const auto& entity : SceneManager::CurrentScene()->Entities)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity->ID);
		for (const auto& component : s)
		{
			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			if (!component->HasScriptAttached)
				continue;

			const auto& script = mScripts[component->Script];
			script->OnUpdate(*lua, dt, component->GetID());
		}
	}
}

void Fracture::ScriptManager::Shutdown()
{

}

void Fracture::ScriptManager::RegisterScript(const LuaScriptRegistry& reg)
{
	mScriptRegister[reg.ID] = reg;
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

std::shared_ptr<Fracture::LuaScript> Fracture::ScriptManager::GetInstanceOfScript(const UUID& id)
{
	if (mScriptRegister.find(id) == mScriptRegister.end())
	{
		FRACTURE_ERROR("Could not find Script");
		return nullptr;
	}

	return std::make_shared<LuaScript>(mScriptRegister[id]);
}

void Fracture::ScriptManager::CreateNewScript(const LuaScriptRegistry& reg)
{
	std::ofstream script(reg.Path);

	script << "--" + reg.Name + ".lua---" << std::endl;

	script << std::endl;
	///main Script Table with properties inner table
	script << reg.Name + " = {" << std::endl;
	script << "Properties = {}" << std::endl;
	script << "}" << std::endl;
	script << std::endl;

	///onStart
	script << "function " + reg.Name + ":OnStart(id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onExit
	script << "function " + reg.Name + ":OnExit(id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onUpdate
	script << "function " + reg.Name + ":OnUpdate(dt,id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onLateUpdate
	script << "function " + reg.Name + ":OnLateUpdate(dt,id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onFixedUpdate
	script << "function " + reg.Name + ":OnFixedUpdate(dt,id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onCollision
	script << "function " + reg.Name + ":OnCollision(id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	///onTrigger
	script << "function " + reg.Name + ":OnTrigger(id)" << std::endl;
	script << "--Start Code --" << std::endl;
	script << "end" << std::endl;
	script << std::endl;

	script << "return " + reg.Name << std::endl;

	script.close();
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

bool Fracture::ScriptManager::HasScript(const UUID& script)
{
	return mScriptRegister.find(script) != mScriptRegister.end();
}
