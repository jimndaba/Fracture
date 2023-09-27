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

#include "Rendering/DebugRenderer.h"

#include "Particles/ParticleSystem.h"
#include "Serialisation/Serialiser.h"
#include "Common/Helpers.h"

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

	log.set_function("DrawLine", [&](sol::this_state s,glm::vec3 start, glm::vec3 end) {
			DebugRenderer::DrawLine(start,end);
		});

	log.set_function("DrawCircle", [&](sol::this_state s, glm::vec3 center, glm::vec3 rotation, float radius) {
		DebugRenderer::DrawCircle(center, radius,rotation);
		});

	log.set_function("DrawSphere", [&](sol::this_state s, glm::vec3 center, float radius) {
		DebugRenderer::DrawSphere(center,radius);
		});

	log.set_function("DrawAABB", [&](sol::this_state s, glm::vec3 min, glm::vec3 max) {
		DebugRenderer::DrawAABB(min, max);
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

	lua.set_function("Vec3Lerp", LuaBindComponents::Vec3Lerp);
	lua.set_function("Vec2Lerp", LuaBindComponents::Vec2Lerp);
	lua.set_function("Vec4Lerp", LuaBindComponents::Vec4Lerp);

	lua.set_function("EmitFx", ParticleSystem::EmittParticles);
	lua.set_function("StopFx", ParticleSystem::EmittParticles);
	
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
	L.set_function("Move", LuaBindPhysics::Move);
	L.set_function("SetLinearVelocity", LuaBindPhysics::SetLinearVelocity);
	L.set_function("SetAngularVelocity", LuaBindPhysics::SetAngularVelocity);
	L.set_function("SetMaxLinearVelocity", LuaBindPhysics::SetMaxLinearVelocity);
	L.set_function("SetMaxAngularVelocity", LuaBindPhysics::SetMaxAngularVelocity);
	L.set_function("SetGravityDisabled", LuaBindPhysics::SetGravityDisabled);
	L.set_function("IsControllerGrounded", LuaBindPhysics::IsControllerGrounded);

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

			if (component->HasStarted)
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

			if (component->HasStarted)
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

void Fracture::ScriptManager::onLateUpdate(float dt)
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
			script->OnLateUpate(*lua.get(), dt, component->GetID());
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
			script->OnLateUpate(*lua.get(), dt, component->GetID());
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

void Fracture::ScriptManager::OnSave()
{
	for (const auto& script : mScripts)
	{
		SaveScriptProperties(script.first);
	}
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

void Fracture::ScriptManager::LoadScript(Fracture::UUID script_id)
{
	if (mScriptRegister.find(script_id) == mScriptRegister.end())
	{
		FRACTURE_ERROR("Could not find Script");
		return;
	}

	if (mScripts.find(script_id) == mScripts.end())
	{
		mScripts[script_id] = std::make_shared<LuaScript>(mScriptRegister[script_id]);
	}
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

void Fracture::ScriptManager::LoadScriptProperties(LuaScript* script)
{
	if (script)
	{
		auto saver = ISerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
		saver.Open(mScriptRegister[script->Description.ID].MetaPath);
		if (saver.BeginStruct("Script"))
		{
			if (saver.BeginCollection("Properties"))
			{
				while (saver.CurrentCollectionIndex() < saver.GetCollectionSize())
				{
					if (saver.BeginStruct("Property"))
					{
						std::string prop_Name = saver.STRING("Name");
						if (script->m_Properties.find(prop_Name) != script->m_Properties.end())
						{
							auto& prop = script->m_Properties[prop_Name];
							
							switch (prop->Type)
							{
							case PROPERTY_TYPE::BOOL:
							{
								prop->Bool = saver.BOOL("Value");
								break;
							}
							case PROPERTY_TYPE::UUID:
							{
							    prop->ID = saver.ID("Value");
								break;
							}
							case PROPERTY_TYPE::FLOAT:
							{
								prop->Float = saver.FLOAT("Value");
								break;
							}
							case PROPERTY_TYPE::INT:
							{
								prop->Int = saver.INT("Value");
								break;
							}
							case PROPERTY_TYPE::VEC2:
							{
								prop->Vec2 = saver.VEC2("Value");
								break;
							}
							case PROPERTY_TYPE::VEC3:
							{
								prop->Vec3 = saver.VEC3("Value");
								break;
							}
							case PROPERTY_TYPE::VEC4:
							{
								prop->Vec4 = saver.VEC4("Value");;
								break;
							}
							}
						}
						saver.EndStruct();
					}
					saver.NextInCollection();
				}
				saver.EndCollection();

			}
			saver.EndStruct();
		}
	}

}

void Fracture::ScriptManager::SaveScriptProperties(const UUID& id)
{
	if (mScripts.find(id) != mScripts.end())
	{
		auto& script = mScripts[id];

		auto saver = ISerialiser(Fracture::ISerialiser::IOMode::Save, Fracture::ISerialiser::SerialiseFormat::Json);	
		saver.BeginStruct("Script");
		saver.Property("Name", script->Description.Name);
		saver.Property("ID", script->Description.ID);
		saver.Property("Script Path", script->Description.Path);

		saver.BeginCollection("Properties");
		for (const auto& prop : script->m_Properties)
		{
			saver.BeginStruct("Property");
			saver.Property("Name", prop.second->Name);
			saver.Property("Type", (int)prop.second->Type);

			switch (prop.second->Type)
			{
			case PROPERTY_TYPE::BOOL:
			{
				saver.Property("Value", prop.second->Bool);
				break;
			}
			case PROPERTY_TYPE::UUID:
			{
				saver.Property("Value", prop.second->ID);
				break;
			}
			case PROPERTY_TYPE::FLOAT:
			{
				saver.Property("Value", prop.second->Float);
				break;
			}
			case PROPERTY_TYPE::INT:
			{
				saver.Property("Value", prop.second->Int);
				break;
			}
			case PROPERTY_TYPE::VEC2:
			{
				saver.Property("Value", prop.second->Vec2);
				break;
			}
			case PROPERTY_TYPE::VEC3:
			{
				saver.Property("Value", prop.second->Vec3);
				break;
			}
			case PROPERTY_TYPE::VEC4:
			{
				saver.Property("Value", prop.second->Vec4);
				break;
			}
			}

			saver.EndStruct();
		}
		saver.EndCollection();
		saver.EndStruct();

		if (script->Description.MetaPath.empty())
		{
			auto new_path = Fracture::Helper::GetDirectoryFromPath(script->Description.Path);
			script->Description.MetaPath = new_path  + script->Description.Name + ".ScriptInfo";
			mScriptRegister[id].MetaPath = script->Description.MetaPath;
		}
		saver.Save(script->Description.MetaPath);
	}
}

void Fracture::ScriptManager::LoadScriptProperties(const UUID& id)
{
	if (mScriptRegister.find(id) != mScriptRegister.end())
	{
		auto& script = mScripts[id];
		if (script)
		{
			auto saver = ISerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
			saver.Open(mScriptRegister[id].MetaPath);

			if (saver.BeginStruct("Script"))
			{
				if (saver.BeginCollection("Properties"))
				{
					while (saver.CurrentCollectionIndex() < saver.GetCollectionSize())
					{
						if (saver.BeginStruct("Property"))
						{
							auto prop_Name = saver.STRING("Name");
							if (script->m_Properties.find(prop_Name) != script->m_Properties.end())
							{
								auto& prop = script->m_Properties[prop_Name];
								switch (prop->Type)
								{
								case PROPERTY_TYPE::BOOL:
								{
									saver.Property("Value", prop->Bool);
									break;
								}
								case PROPERTY_TYPE::UUID:
								{
									saver.Property("Value", prop->ID);
									break;
								}
								case PROPERTY_TYPE::FLOAT:
								{
									saver.Property("Value", prop->Float);
									break;
								}
								case PROPERTY_TYPE::INT:
								{
									saver.Property("Value", prop->Int);
									break;
								}
								case PROPERTY_TYPE::VEC2:
								{
									saver.Property("Value", prop->Vec2);
									break;
								}
								case PROPERTY_TYPE::VEC3:
								{
									saver.Property("Value", prop->Vec3);
									break;
								}
								case PROPERTY_TYPE::VEC4:
								{
									saver.Property("Value", prop->Vec4);
									break;
								}
								}
							}
							saver.EndStruct();
						}
						saver.NextInCollection();
					}
					saver.EndCollection();

				}
				saver.EndStruct();
			}
		}		
	}
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

