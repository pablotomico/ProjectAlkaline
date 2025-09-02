#include "raylib.h"
#include "systems/Script/ScriptSystem.h"
#include "systems/GameLogic/GameLogic.h"
#include "components/ScriptComponent.h"

alk::ScriptSystem::ScriptSystem() : safe(luaState)
{
    luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
    luaState.set_panic([](lua_State* L) {
        const char* message = lua_tostring(L, -1);
        ALK_ERROR("LuaError: %s", message);
        DEBUG_BREAK();
        return 0; });
    GetState() = &luaState;
    GetSafeRunner() = &safe;
    GetNotificationMap() = &notificationMap;
}

void alk::ScriptSystem::Initialize(Scene& scene)
{
    alk::ScriptSystem::RegisterNotification("TestNotification");

    alk::ScriptSystem::CreateNamespace("System")
        .AddFunction("ConnectToNotification", ScriptSystem::RegisterNotificationCallback);

    auto components = scene.GetWorld().GetComponents<ScriptComponent>();
    for (int i = 0; i < components->Size(); ++i)
    {
        auto [entityId, scriptComponent] = components->Get(i);
        auto result = RunFile(scriptComponent->path);
        sol::type t = result.get_type();
        if (t == sol::type::table)
        {
            sol::table table = result;
            scriptComponent->self = table;
            scriptComponent->onStart = table.get<sol::function>("OnStart");
            scriptComponent->onUpdate = table.get<sol::function>("OnUpdate");
            scriptComponent->onStop = table.get<sol::function>("OnStop");

            scriptComponent->self.set("owner", entityId);

            if (scriptComponent->onStart)
            {
                CallFunction(scriptComponent->onStart, scriptComponent->self, entityId);
            }
        }
    }
}

void alk::ScriptSystem::Reflect(ScriptSystem& script)
{
}

void alk::ScriptSystem::Update(float deltaTime)
{
    for (auto& scriptComponent : *alk::GameLogic::GetWorld().GetComponents<ScriptComponent>())
    {
        if (scriptComponent.onUpdate)
        {
            CallFunction(scriptComponent.onUpdate, scriptComponent.self, deltaTime);
        }
    }
}

void alk::ScriptSystem::Shutdown() {}

sol::state*& alk::ScriptSystem::GetState()
{
    static sol::state* luaState;
    return luaState;
}

alk::LuaSafeRunner*& alk::ScriptSystem::GetSafeRunner()
{
    static LuaSafeRunner* safe;
    return safe;
}

alk::NotificationMap*& alk::ScriptSystem::GetNotificationMap()
{
    static alk::NotificationMap* notificationMap;
    return notificationMap;
}

void alk::ScriptSystem::RegisterNotification(const std::string& notification)
{
    auto notificationMap = GetNotificationMap();
    ALK_ASSERT(!notificationMap->contains(notification), "[ScriptSystem] Notification already registered: %s", notification.c_str());
    notificationMap->emplace(notification, std::vector<sol::function>());
}

void alk::ScriptSystem::RegisterNotificationCallback(std::string notification, sol::function callback)
{
    auto notificationMap = GetNotificationMap();
    if (notificationMap->contains(notification))
    {
        notificationMap->at(notification).push_back(callback);
    }
}

void alk::ScriptSystem::AddToPackage(const std::string& path)
{
    luaState["package"]["path"] = luaState["package"]["path"].get<std::string>() + ";" + GetWorkingDirectory() + path + "?.lua";
    ALK_LOG("ScriptSystem::AddToPackage: '%s'", path.c_str());
}

sol::table alk::ScriptSystem::LoadTableFromFile(const std::string& filePath)
{
    sol::load_result script = GetState()->load_file(filePath);
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