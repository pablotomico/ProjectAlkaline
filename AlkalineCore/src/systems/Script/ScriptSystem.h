#pragma once

#include "systems/BaseSystem.h"
#include "systems/Script/ScriptSystemHelpers.h"
#include <string>
#include <map>

#include "raylib/raylib.h"
#include "alkaline_lib.h"


namespace alk
{
    using NotificationMap = std::map<std::string, std::vector<sol::function>>;

    class ScriptSystem : public BaseSystem
    {
    public:
        ScriptSystem(CoreSystems& coreSystems);

        void Initialize(Scene& scene) override;
        void Reflect(ScriptSystem& script) override;
        void Update(const float deltaTime) override;
        void Shutdown() override;

        static sol::state*& GetState();
        static LuaSafeRunner*& GetSafeRunner();
        static NotificationMap*& GetNotificationMap();
        void RegisterNotification(const std::string& notification);
        void RegisterNotificationCallback(std::string notification, sol::function callback);

        void AddToPackage(const std::string& path);
        static sol::table LoadTableFromFile(const std::string& filePath);
        static std::string TableToString(const sol::table& table, int indent = 1);
        static void SaveTableToFile(const std::string& filePath, const sol::table& table);

        template <typename... Args>
        inline static void SendNotification(const std::string& notification, Args&&... args)
        {
            auto notificationMap = GetNotificationMap();
            ALK_ASSERT(notificationMap->contains(notification), "[ScriptSystem] Attempting to send nonexisting notification: %s", notification.c_str());
            LuaSafeRunner* safe = GetSafeRunner();
            //IDEA: buffer notifications and send in Update
            for (auto& callback : notificationMap->at(notification))
            {
                safe->CallFunction(callback, std::forward<Args>(args)...);
            }
        }



        inline static sol::object RunFile(const std::string& filename)
        {
            return GetSafeRunner()->RunFileSafe(std::string(GetWorkingDirectory()) + "/" + filename);
        }

        template <typename... Args>
        bool CallFunction(const std::string& funcName, Args&&... args)
        {
            return GetSafeRunner()->CallFunction(funcName, std::forward<Args>(args)...);
        }

        template <typename... Args>
        bool CallFunction(sol::function fn, Args&&... args)
        {
            return GetSafeRunner()->CallFunction(fn, std::forward<Args>(args)...);
        }

        inline LuaNamespace CreateNamespace(const std::string& name)
        {
            return LuaNamespace(*GetState(), name);
        }

        template<typename T>
        LuaUsertype<T> CreateUsertype(const std::string& name)
        {
            return LuaUsertype<T>(*GetState(), name);
        }

    private:
        sol::state luaState;
        LuaSafeRunner safe;
        NotificationMap notificationMap;
    };
}