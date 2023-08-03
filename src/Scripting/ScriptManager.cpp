#include "FracturePCH.h"
#include "ScriptManager.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "Input/Input.h"
#include "LuaScript.h"
#include "EventSystem/Eventbus.h"

#include "LuaMathBindings.h"
#include "LuaComponentBindings.h"
#include "LuaPhysicsBindings.h"

#include "Physics/PhysicsEvents.h"
#include "World/WorldEvents.h"

std::unique_ptr<sol::state> Fracture::ScriptManager::lua;
std::map<Fracture::UUID, Fracture::LuaScriptRegistry> Fracture::ScriptManager::mScriptRegister;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::LuaScript>> Fracture::ScriptManager::mScripts;

Fracture::ScriptManager::ScriptManager()
{

}

void Fracture::ScriptManager::BindLog(sol::state& L)
{
	auto log = L.create_table("Debug");
	log.set_function("log", sol::overload([&](sol::this_state s, std::string message) {
		FRACTURE_INFO(message);
		}));
	
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
	lua.new_usertype<Fracture::UUID>("UUID",
		// Constructor 
	// Constructor 
		sol::constructors <
		Fracture::UUID()		
		>(),

		"GetValue", & UUID::GetValue,

		sol::meta_function::to_string, [](Fracture::UUID& v) {return fmt::format("UUID({})", std::to_string((uint32_t) v)); }
		
		);


	lua.set_function("UUID", []() {return Fracture::UUID(); });

	lua.set_function("IsTaggedWith", SceneManager::IsTaggedWith);
	lua.set_function("GetEntityWithTag", SceneManager::GetEntityWithTag);
}

void Fracture::ScriptManager::BindFunctions(sol::state& lua)
{	
	lua.set_function("SetPosition", LuaBindComponents::SetPosition);
	lua.set_function("SetScale", LuaBindComponents::SetScale);
	lua.set_function("SetRotation", LuaBindComponents::SetRotation);
	lua.set_function("GetPosition", LuaBindComponents::GetPosition);
	lua.set_function("GetScale", LuaBindComponents::GetScale);
	lua.set_function("GetRotation", LuaBindComponents::GetRotation);
	lua.set_function("Translate", LuaBindComponents::Translate);
	lua.set_function("Rotate", LuaBindComponents::Rotate);
	lua.set_function("LookAt", LuaBindComponents::LookAt);
	lua.set_function("Instantiate", LuaBindComponents::Instantiate);

	lua.set_function("Destroy", SceneManager::Destroy);
	
	/*
	lua.set_function("Destroy", sol::overload([&](sol::this_state s, UUID id) {
		return SceneManager::RemoveEntity(id);
		}));
	*/
}


void Fracture::ScriptManager::BindInput(sol::state& L)
{
	auto input = L.create_table("Input");
	input.set_function("IsKeyDown", [](Fracture::KeyCode key) -> bool {
		return Input::IsKeyDown(key);
		});

	input.set_function("IsKeyUp", [](Fracture::KeyCode key) -> bool {
		return Input::IsKeyUp(key);
		});

	input.set_function("GetMousePosition", [](){
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
	LuaBindGLM::BindVec2(&L);
	LuaBindGLM::BindVec3(&L);
	LuaBindGLM::BindVec4(&L);
}

void Fracture::ScriptManager::BindPhysics(sol::state& L)
{
	L.set_function("RotateRigidbody", LuaBindPhysics::RotateRigidBody);
	L.set_function("SetLinearVelocity", LuaBindPhysics::SetLinearVelocity);
	L.set_function("SetAngularVelocity", LuaBindPhysics::SetAngularVelocity);
	L.set_function("SetMaxLinearVelocity", LuaBindPhysics::SetMaxLinearVelocity);
	L.set_function("SetMaxAngularVelocity", LuaBindPhysics::SetMaxAngularVelocity);
	L.set_function("SetGravityDisabled", LuaBindPhysics::SetGravityDisabled);

	L.set_function("GetAngularVelocity", LuaBindPhysics::GetAngularVelocity);
	L.set_function("GetLinearVelocity", LuaBindPhysics::GetLinearVelocity);
	L.set_function("GetMaxAngularVelocity", LuaBindPhysics::GetMaxAngularVelocity);
	L.set_function("GetMaxLinearVelocity", LuaBindPhysics::GetMaxLinearVelocity);

	L.set_function("AddForce", LuaBindPhysics::AddForce);
	L.set_function("AddTorque", LuaBindPhysics::AddTorque);
}

void Fracture::ScriptManager::BindApplication(sol::state& L)
{

}

void Fracture::ScriptManager::Init()
{
	lua = std::make_unique<sol::state>();
	
	lua->open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::string,
		sol::lib::os,
		sol::lib::math,
		sol::lib::table,
		sol::lib::debug,
		sol::lib::bit32,
		sol::lib::io
	);
	//lua->clear_package_loaders();

	BindCore(*lua.get());
	BindLog(*lua.get());
	BindInput(*lua.get());
	BindFunctions(*lua.get());
	BindMaths(*lua.get());
	BindPhysics(*lua.get());
	LuaBindPhysics::BindPhysicsEvents(lua.get());

	Fracture::Eventbus::Subscribe(this, &ScriptManager::OnCollision);

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
			script->OnStart(*lua.get(), component->GetID());
			component->HasStarted = true;
		}
	}	

	for (const auto& entity : SceneManager::CurrentScene()->mPrefabs)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity.PrefabID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnStart(*lua.get(), component->GetID());
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
			{
				continue;
			}

			const auto& script = mScripts[component->Script];
			script->OnExit(*lua.get(), component->GetID());
		}
	}

	for (const auto& entity : SceneManager::CurrentScene()->mPrefabs)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity.PrefabID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnExit(*lua.get(),component->GetID());
			component->HasStarted - false;
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
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;
			
			const auto& script = mScripts[component->Script];
			script->OnUpdate(*lua.get(), dt, component->GetID());
		}
	}

	for (const auto& entity : SceneManager::CurrentScene()->mPrefabs)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity.PrefabID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnUpdate(*lua.get(), dt, component->GetID());
		}
	}
}

void Fracture::ScriptManager::onFixedUpdate()
{
	for (const auto& entity : SceneManager::CurrentScene()->Entities)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity->ID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnFixedUpdate(*lua.get(),1/60.0f, component->GetID());
		}
	}

	for (const auto& entity : SceneManager::CurrentScene()->mPrefabs)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity.PrefabID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnFixedUpdate(*lua.get(), 1 / 60.0f, component->GetID());
		}
	}
}

void Fracture::ScriptManager::Shutdown()
{
	mScripts.clear();
	lua.reset();
}

void Fracture::ScriptManager::Instantiate(UUID Entity, glm::vec3 position)
{
	Eventbus::Publish<InstantiatePrefabEvent>(Entity, position);
}

void Fracture::ScriptManager::OnCollision(const std::shared_ptr<OnCollisionEvent>& evnt)
{
	for (const auto& entity : SceneManager::CurrentScene()->Entities)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity->ID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnCollision(*lua.get(), evnt->Collision);
		}
	}

	for (const auto& entity : SceneManager::CurrentScene()->mPrefabs)
	{
		const auto& s = SceneManager::GetAllEntityScripts(entity.PrefabID);
		for (const auto& component : s)
		{
			if (!component->HasScriptAttached)
				continue;

			if (mScripts.find(component->Script) == mScripts.end())
				continue;

			const auto& script = mScripts[component->Script];
			script->OnCollision(*lua.get(), evnt->Collision);
		}
	}
}

void Fracture::ScriptManager::RegisterScript(const LuaScriptRegistry& reg)
{
	mScriptRegister[reg.ID] = reg;
}

void Fracture::ScriptManager::Reload(LuaScript* mscript)
{
	FRACTURE_INFO("Script {} Reloaded", mscript->Description.Name);
	mscript->Reload(*lua.get());
	mscript->IsStarted(false);
}

void Fracture::ScriptManager::LoadScript(const std::shared_ptr<LuaScript>& mscript)
{
	mscript->Load(*lua.get());
	mscript->IsStarted(false);
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

std::shared_ptr<Fracture::LuaScript> Fracture::ScriptManager::GetLuaScript(const UUID& id)
{
	if (mScriptRegister.find(id) == mScriptRegister.end())
	{
		FRACTURE_ERROR("Could not find Script");
		return nullptr;
	}
	
	if (mScripts.find(id) == mScripts.end())
	{
		mScripts[id] = std::make_shared<LuaScript>(mScriptRegister[id]);
		return mScripts[id];
	}
	else if (mScripts.find(id) != mScripts.end())
	{
		return mScripts[id];
	}
	return std::shared_ptr<LuaScript>();
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
	script << "function " + reg.Name + ":OnCollision(contactInfo)" << std::endl;
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
	return lua.get();
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

