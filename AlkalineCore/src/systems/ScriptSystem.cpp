#include "systems/ScriptSystem.h"

bool alk::ScriptSystem::Initialize()
{
    sol::state& lua = GetState();
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);
    lua.set_panic([](lua_State *L)
                {
        const char* message = lua_tostring(L, -1);
        ALK_ERROR("LuaError: ", message);
        return 0; });

    return true;
}

sol::table alk::ScriptSystem::LoadTableFromFile(const std::string& filePath)
{
    sol::state& lua = GetState();
    
    sol::load_result script = lua.load_file(filePath);
    if (!script.valid())
    {
        ALK_ERROR("ScriptSystem::LoadTableFromFile: failed to load file with path: '%s'", filePath.c_str());
        return sol::lua_nil;
    }

    sol::table result = script();
    ALK_LOG("ScriptSystem::LoadTableFromFile: loaded file with path: '%s'", filePath.c_str());
    return result;
}