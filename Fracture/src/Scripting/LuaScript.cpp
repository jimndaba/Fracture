#include "LuaScript.h"
#include "Logging/Logger.h"

Fracture::LuaScript::LuaScript(const std::string& name, const std::string& path):
    m_name(name),m_filepath(path+name+".lua"),
    m_Properties(std::make_shared<std::unordered_map<std::string, std::shared_ptr<ScriptProperty>>>())
{
   
}

void Fracture::LuaScript::onStart(sol::state& state)
{
    if(m_onStart)
    { 
        auto self = state[m_name];
        sol::protected_function_result result = m_onStart->call(self);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::onExit(sol::state& state)
{
    if(m_onExit)
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
}

void Fracture::LuaScript::onUpdate(sol::state& state,float dt)
{
    if(m_onUpdate)
    { 
        auto self = state[m_name];
        sol::protected_function_result result = m_onUpdate->call(self,dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::onLateUpate(sol::state& state,float dt)
{
    if(m_onLateUpdate)
    { 
        auto self = state[m_name];
        sol::protected_function_result result = m_onLateUpdate->call(self, dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::onFixedUpdate(sol::state& state,float dt)
{
    if(m_onFixedUpdate)
    {
        auto self = state[m_name];
        sol::protected_function_result result = m_onFixedUpdate->call(self, dt);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::onCollision(sol::state& state)
{
    if(m_onCollision)
    { 
        auto self = state[m_name];
        sol::protected_function_result result = m_onCollision->call(self);
        if (!result.valid())
        {
            sol::error err = result;
            FRACTURE_ERROR("Failed to Execute Script Lua OnUpdate function");
            FRACTURE_ERROR("Error : {0}", err.what());
        }
    }
}

void Fracture::LuaScript::onTrigger(sol::state& state)
{
    if(m_onTrigger)
    {
        auto self = state[m_name];
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
    state.script_file(m_filepath);
    BindFunction(state);
    BindProperties(state);
}

void Fracture::LuaScript::Reload(sol::state& state)
{
    state.do_file(m_filepath);
    BindFunction(state);
    BindProperties(state);
}

std::string Fracture::LuaScript::GetFilepath()
{
    return m_filepath;
}

std::string Fracture::LuaScript::GetName()
{
    return m_name;
}

std::shared_ptr<Fracture::LuaScript> Fracture::LuaScript::Create(const std::string& name, const std::string& path)
{

    std::ofstream script(path + name + ".lua");

    script << "--" + name + ".lua---" << std::endl;
   
    script << std::endl;
       ///main Script Table with properties inner table
    script << name +" = {" << std::endl;
    script << "Properties = {}" << std::endl;
    script << "}" << std::endl; 
    script << std::endl;
    ///onStart
    script << "function "+ name +":onStart()" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onExit
    script << "function " + name + ":onExit()" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onUpdate
    script << "function " + name + ":onUpdate(dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onLateUpdate
    script << "function " + name + ":onLateUpdate(dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onFixedUpdate
    script << "function " + name + ":onFixedUpdate(dt)" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onCollision
    script << "function " + name + ":onCollision()" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    ///onTrigger
    script << "function " + name + ":onTrigger()" << std::endl;
    script << "--Start Code --" << std::endl;
    script << "end" << std::endl;
    script << std::endl;

    script << "return " + name << std::endl;

    script.close();

    return std::make_shared<LuaScript>(name,path);
}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Fracture::ScriptProperty>>> Fracture::LuaScript::GetProperties()
{
    return m_Properties;
}

void Fracture::LuaScript::BindFunction(sol::state& state)
{   
    auto self = state[m_name];    
    m_onStart = std::make_shared<sol::protected_function>(self["onStart"]);
    if (!m_onStart->valid())
        m_onStart.reset();

    m_onExit =  std::make_shared<sol::protected_function>(self["onExit"]);
    if (!m_onExit->valid())
        m_onExit.reset();

    m_onUpdate = std::make_shared<sol::protected_function>(self["onUpdate"]);
    if (!m_onUpdate->valid())
        m_onUpdate.reset();

    m_onLateUpdate = std::make_shared<sol::protected_function>(self["onLateUpdate"]);
    if (!m_onLateUpdate->valid())
        m_onLateUpdate.reset();

    m_onFixedUpdate = std::make_shared<sol::protected_function>(self["onFixedUpdate"]);
    if (!m_onFixedUpdate->valid())
        m_onFixedUpdate.reset();

    m_onCollision = std::make_shared<sol::protected_function>(self["onCollision"]);
    if (!m_onCollision->valid())
        m_onCollision.reset();

    m_onTrigger = std::make_shared<sol::protected_function>(self["onTrigger"]);
    if (!m_onTrigger->valid())
        m_onTrigger.reset();

}

void Fracture::LuaScript::BindProperties(sol::state& state)
{
    m_Properties->clear();

    sol::optional<sol::table> self = state[m_name]["Properties"];
    //sol::optional<sol::table> conf = lua["conf"];
    //if (conf) {
    //    for (const auto& key_value_pair : conf.value())
    //    {
    //        sol::object key = key_value_pair.first;
    //        sol::object value = key_value_pair.second;
    //        std::string k = key.as<std::string>();
    //        sol::type t = value.get_type();
    //        switch (t) {
    //        case sol::type::string: {
    //            std::cout << k << ": " << value.as<std::string>() << std::endl;
    //        }
    //                              break;
    //        case sol::type::number: {
    //            auto v = value.as<double>();
    //            std::cout << k << ": " << v << std::endl;
    //        }
    //                              break;
    //        default: {
    //            std::cout << "hit the default case!" << std::endl;
    //        }
    //        }
    //    }
    //}

    for (const auto& prop : self.value())
    {
        sol::object key = prop.first;
        sol::object mvalue = prop.second;

        std::string k = key.as<std::string>();       
        sol::type t = mvalue.get_type();
        switch (t)
        {
        case sol::type::string: 
        {
            std::shared_ptr<ScriptProperty> prop = std::make_shared<ScriptProperty>();
            prop->Name = k;
            prop->Type = PROPERTY_TYPE::STRING;
            prop->String = mvalue.as<std::string>().c_str();       
            m_Properties->emplace(k, prop);         
        }
        break;
        case sol::type::number: 
            {
                std::shared_ptr<ScriptProperty> prop = std::make_shared<ScriptProperty>();
                prop->Name = k;
                prop->Type = PROPERTY_TYPE::FLOAT;
                prop->Float = mvalue.as<double>();               
                m_Properties->emplace(k, prop);              
            }
        break;        
        }

    }
}
