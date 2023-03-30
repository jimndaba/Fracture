#include "FracturePCH.h"
#include "LuaScript.h"

Fracture::LuaScript::LuaScript(const LuaScriptRegistry& reg):Description(reg)
{
}


void Fracture::LuaScript::OnStart(sol::state& state, const UUID& id)
{
    if (m_onStart)
    {
        auto self = state[m_name];
        auto result = m_onStart->call(self, id);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
    isStarted = true;
}

void Fracture::LuaScript::OnExit(sol::state& state, const UUID& id)
{
    if (m_onExit)
    {
        auto self = state[m_name];
        sol::protected_function_result result = m_onExit->call(self);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
    isStarted = false;
}


void Fracture::LuaScript::OnUpdate(sol::state& state, const UUID& id, float dt)
{
    if (m_onUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onUpdate->call(self, id, dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnLateUpate(sol::state& state, const UUID& id, float dt)
{
    if (m_onLateUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onLateUpdate->call(self, dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnFixedUpdate(sol::state& state, const UUID& id, float dt)
{
    if (m_onFixedUpdate)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onFixedUpdate->call(self, dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnCollision(sol::state& state)
{
    if (m_onCollision)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onCollision->call(self);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::OnTrigger(sol::state& state)
{
    if (m_onTrigger)
    {
        auto self = state[Description.Name];
        sol::protected_function_result result = m_onTrigger->call(self);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
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
}

bool Fracture::LuaScript::IsStarted() const
{
    return isStarted;

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
            auto prop = ScriptProperty();
            prop.Name = k;
            prop.Type = PROPERTY_TYPE::BOOL;
            prop.Bool = mvalue.as<bool>();
            m_Properties.emplace(k, prop);
        }
        break;
        case sol::type::string:
        {
            auto prop = ScriptProperty();
            prop.Name = k;
            prop.Type = PROPERTY_TYPE::STRING;
            prop.String = mvalue.as<std::string>().c_str();
            m_Properties.emplace(k, prop);
        }
        break;
        case sol::type::number:
        {
            auto prop = ScriptProperty();
            prop.Name = k;
            prop.Type = PROPERTY_TYPE::FLOAT;
            prop.Float = mvalue.as<float>();
            m_Properties.emplace(k, prop);
        }
        break;
        case sol::type::userdata:
        {
            auto prop = ScriptProperty();
            if (mvalue.is<glm::vec2>())
            {
                prop.Type = PROPERTY_TYPE::VEC2;
                prop.Vec2 = mvalue.as<glm::vec2>();
            }
            if (mvalue.is<glm::vec3>())
            {
                prop.Type = PROPERTY_TYPE::VEC3;
                prop.Vec3 = mvalue.as<glm::vec3>();
            }
            if (mvalue.is<glm::vec4>())
            {
                prop.Type = PROPERTY_TYPE::VEC4;
                prop.Vec4 = mvalue.as<glm::vec4>();
            }
            prop.Name = k;
            m_Properties.emplace(k, prop);
        }
        break;
        }

    }
}

void Fracture::LuaScript::OnPropertyUpdate(sol::state& state, const Fracture::ScriptProperty& prop)
{
    switch (prop.Type)
    {
    case PROPERTY_TYPE::STRING:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.String;
    }
    break;
    case PROPERTY_TYPE::BOOL:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Bool;
    }
    break;
    case PROPERTY_TYPE::FLOAT:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Float;
    }
    break;
    case PROPERTY_TYPE::VEC2:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec2;
    }
    break;
    case PROPERTY_TYPE::VEC3:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec3;
    }
    break;
    case PROPERTY_TYPE::VEC4:
    {
        state[Description.Name]["Properties"][prop.Name] = prop.Vec4;
    }
    break;
    }
}

std::shared_ptr<Fracture::LuaScript> Fracture::LuaScript::Create(const std::string& name, const std::string& path)
{
    std::ofstream script(path);

    script << "--" + name + ".lua---" << std::endl;

    script << std::endl;
    ///main Script Table with properties inner table
    script << name + " = {" << std::endl;
    script << "Properties = {}" << std::endl;
    script << "}" << std::endl;
    script << std::endl;

    ///onStart
    script << "function " + name + ":OnStart(id)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onExit
    script << "function " + name + ":OnExit(id)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onUpdate
    script << "function " + name + ":OnUpdate(id,dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onLateUpdate
    script << "function " + name + ":OnLateUpdate(id,dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onFixedUpdate
    script << "function " + name + ":OnFixedUpdate(id,dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onCollision
    script << "function " + name + ":OnCollision(id)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onTrigger
    script << "function " + name + ":OnTrigger(id)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    script << "return " + name << std::endl;

    script.close();
   
    LuaScriptRegistry reg;
    reg.Name = name;
    reg.Path = path;
    reg.ID = UUID();
    return std::make_shared<LuaScript>(reg);
}

std::unordered_map<std::string, Fracture::ScriptProperty> Fracture::LuaScript::GetProperties()
{
    return m_Properties;
}