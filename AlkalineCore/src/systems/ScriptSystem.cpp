#include "systems/ScriptSystem.h"

#include "raylib.h"

bool alk::ScriptSystem::Initialize()
{
    sol::state& lua = GetState();
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
    lua.set_panic([](lua_State* L) {
        const char* message = lua_tostring(L, -1);
        ALK_ERROR("LuaError: %s", message);
        return 0; });

    RegisterNotification("TestNotification");

    CreateNamespace("System")
        .AddFunction("ConnectToNotification", RegisterNotificationCallback);
    return true;
}

void alk::ScriptSystem::AddToPackage(const std::string& path)
{
    sol::state& lua = GetState();
    lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + GetWorkingDirectory() + path + "?.lua";
    ALK_LOG("ScriptSystem::AddToPackage: '%s'", path.c_str());
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
    // ALK_LOG("ScriptSystem::LoadTableFromFile: loaded file with path: '%s'", filePath.c_str());
    return result;
}


void alk::ScriptSystem::SaveTableToFile(const std::string& filePath, const sol::table& table)
{
    std::ostringstream oss;
    std::string s = TableToString(table);
    oss << "return " << s;
    SaveFileText(filePath.c_str(), oss.str().data());
}

std::string alk::ScriptSystem::TableToString(const sol::table& table, int indent)
{
    std::ostringstream oss;
    std::string ind(indent * 4, ' ');
    oss << "{\n";

    for (auto& kvp : table)
    {
        sol::object key = kvp.first;
        sol::object value = kvp.second;

        oss << ind;

        // Keys
        if (key.is<std::string>())
        {
            oss << key.as<std::string>() << " = ";
        }
        else if (key.is<int>())
        {
            oss << "[" << key.as<int>() << "] = ";
        }
        else
        {
            oss << "[unknown_key] = ";
        }

        // Values
        if (value.is<sol::table>())
        {
            oss << TableToString(value.as<sol::table>(), indent + 1);
        }
        else if (value.is<std::string>())
        {
            oss << "\"" << value.as<std::string>() << "\"";
        }
        else if (value.is<int>())
        {
            oss << value.as<int>();
        }
        else if (value.is<double>())
        {
            oss << value.as<double>();
        }
        else if (value.is<bool>())
        {
            oss << (value.as<bool>() ? "true" : "false");
        }
        else if (value.is<sol::lua_nil_t>())
        {
            oss << "nil";
        }
        else
        {
            oss << "\"[unsupported_type]\"";
        }

        oss << ",\n";
    }

    oss << std::string((indent - 1) * 4, ' ') << "}";
    return oss.str();
}