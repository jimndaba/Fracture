#include "FracturePCH.h"
#include "LuaScript.h"
#include "Physics/PhysicsEvents.h"

Fracture::LuaScript::LuaScript(const LuaScriptRegistry& reg):Description(reg)
{
}


void Fracture::LuaScript::OnStart(sol::state& state,const Fracture::UUID& entity)
{
    if (m_onStart)
    {
        auto self = state[Description.Name];
        auto result = m_onStart->call(self,entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnStart function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
    isStarted = true;
}

void Fracture::LuaScript::OnExit(sol::state& state, const Fracture::UUID& entity)
{
    if (m_onExit)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onExit->call(self, entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnExit function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
    isStarted = false;
}

void Fracture::LuaScript::OnUpdate(sol::state& state, float dt, const Fracture::UUID& entity)
{
    if (m_onUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onUpdate->call(self, dt, entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnLateUpate(sol::state& state, float dt, const Fracture::UUID& entity)
{
    if (m_onLateUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onLateUpdate->call(self, dt, entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnLateUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnFixedUpdate(sol::state& state, float dt, const Fracture::UUID& entity)
{
    if (m_onFixedUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onFixedUpdate->call(self, dt, entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnFixedUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnCollision(sol::state& state, const Fracture::CollisionContext& cntxt)
{
    if (m_onCollision)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onCollision->call(self, cntxt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnCollision function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnTrigger(sol::state& state, const Fracture::UUID& entity)
{
    if (m_onTrigger)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onTrigger->call(self, entity);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnTrigger function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::DoScript(sol::state& state)
{
    state.script_file(Description.Path);
    BindFunctions(state);
    BindProperties(state);
}

void Fracture::LuaScript::Load(sol::state& state)
{
    state.script_file(Description.Path);
    BindFunctions(state);
    BindProperties(state);
}

void Fracture::LuaScript::Reload(sol::state& state)
{
    state.do_file(Description.Path);
    BindFunctions(state);
    BindProperties(state);
    FRACTURE_TRACE("Reloaded Script");
}

bool Fracture::LuaScript::IsStarted() const
{
    return isStarted;

}

void Fracture::LuaScript::IsStarted(bool started)
{
    isStarted = started;
}

void Fracture::LuaScript::BindFunctions(sol::state& state)
{
    auto self = state[Description.Name];
    m_onStart = std::make_shared<sol::protected_function>(self["OnStart"]);
    if (!m_onStart->valid())
        m_onStart.reset();

    m_onExit = std::make_shared<sol::protected_function>(self["OnExit"]);
    if (!m_onExit->valid())
        m_onExit.reset();

    m_onUpdate = std::make_shared<sol::protected_function>(self["OnUpdate"]);
    if (!m_onUpdate->valid())
        m_onUpdate.reset();

    m_onLateUpdate = std::make_shared<sol::protected_function>(self["OnLateUpdate"]);
    if (!m_onLateUpdate->valid())
        m_onLateUpdate.reset();

    m_onFixedUpdate = std::make_shared<sol::protected_function>(self["OnFixedUpdate"]);
    if (!m_onFixedUpdate->valid())
        m_onFixedUpdate.reset();

    m_onCollision = std::make_shared<sol::protected_function>(self["OnCollision"]);
    if (!m_onCollision->valid())
        m_onCollision.reset();

    m_onTrigger = std::make_shared<sol::protected_function>(self["OnTrigger"]);
    if (!m_onTrigger->valid())
        m_onTrigger.reset();

}

void Fracture::LuaScript::BindProperties(sol::state& state)
{
    m_Properties.clear();
    sol::optional<sol::table> self = state[Description.Name]["Properties"];
    for (const auto& prop : self.value())
    {
        sol::object key = prop.first;
        sol::object mvalue = prop.second;

        std::string k = key.as<std::string>();
        sol::type t = mvalue.get_type();
        switch (t)
        {
            case sol::type::boolean:
            {
                auto prop = std::make_shared<ScriptProperty>();
                prop->Name = k;
                prop->Type = PROPERTY_TYPE::BOOL;
                prop->Bool = mvalue.as<bool>();
                m_Properties.push_back(prop);
                break;
            }
      
            case sol::type::string:
            {
                auto prop = std::make_shared<ScriptProperty>();
                prop->Name = k;
                prop->Type = PROPERTY_TYPE::STRING;
                prop->String = mvalue.as<std::string>();
                m_Properties.push_back(prop);
                break;
            }
            case sol::type::number:
            {
                auto prop = std::make_shared<ScriptProperty>();
                prop->Name = k;
                prop->Type = PROPERTY_TYPE::FLOAT;
                prop->Float = mvalue.as<float>();
                m_Properties.push_back(prop);
                break;
            }
            case sol::type::userdata:
            {
                auto prop = std::make_shared<ScriptProperty>();
                if (mvalue.is<glm::vec2>())
                {
                    prop->Type = PROPERTY_TYPE::VEC2;
                    prop->Vec2 = mvalue.as<glm::vec2>();
                }
                if (mvalue.is<glm::vec3>())
                {
                    prop->Type = PROPERTY_TYPE::VEC3;
                    prop->Vec3 = mvalue.as<glm::vec3>();
                }
                if (mvalue.is<glm::vec4>())
                {
                    prop->Type = PROPERTY_TYPE::VEC4;
                    prop->Vec4 = mvalue.as<glm::vec4>();
                }
                if (mvalue.is<Fracture::UUID>())
                {
                    prop->Type = PROPERTY_TYPE::UUID;
                    prop->ID = mvalue.as<Fracture::UUID>();
                }
                prop->Name = k;
                m_Properties.push_back(prop);
                break;
            }
      
        }

    }
}

void Fracture::LuaScript::OnPropertyUpdate(sol::state& state, const Fracture::ScriptProperty& prop)
{
    switch (prop.Type)
    {
    case PROPERTY_TYPE::UUID:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.ID;
        FRACTURE_TRACE("Updated ID: {}", prop.ID);
        break;
    }
    case PROPERTY_TYPE::STRING:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.String;
        break;
    }
  
    case PROPERTY_TYPE::BOOL:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Bool;
        break;
    }

    case PROPERTY_TYPE::FLOAT:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Float;
        break;
    }
    
    case PROPERTY_TYPE::VEC2:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec2;
        break;
    }

    case PROPERTY_TYPE::VEC3:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec3;
        break;
    }

    case PROPERTY_TYPE::VEC4:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec4;
        break;
    }
   
    }
}

std::vector<std::shared_ptr<Fracture::ScriptProperty>> Fracture::LuaScript::GetProperties()
{
    return m_Properties;
}