#pragma once

#include <string>
#include <map>

#include "sol.hpp"

#include "alkaline_lib.h"

namespace alk
{
    namespace ScriptSystem
    {
        class LuaSafeRunner
        {
        public:
            LuaSafeRunner(sol::state& lua) : lua(lua) {}

            // Run a Lua file with traceback
            bool RunFileSafe(const std::string& filename)
            {
                return RunWithTraceback([&]() {
                    return lua.script_file(filename, sol::script_pass_on_error);
                    });
            }

            // Call a Lua function by name with traceback
            template <typename... Args>
            bool CallFunction(const std::string& funcName, Args&&... args)
            {
                return RunWithTraceback([&]() {
                    sol::protected_function func = lua[funcName];
                    if (!func.valid())
                    {
                        ALK_ERROR("LuaRuntime: '%s' is not a valid function", funcName);
                        return sol::protected_function_result();
                    }
                    return func(std::forward<Args>(args)...);
                    });
            }

            template <typename... Args>
            bool CallFunction(sol::function func, Args&&... args)
            {
                return RunWithTraceback([&]() {
                    if (!func.valid())
                    {
                        return sol::protected_function_result();
                    }
                    return func(std::forward<Args>(args)...);
                    });
            }

        private:
            sol::state& lua;

            template <typename Callable>
            bool RunWithTraceback(Callable&& action)
            {
                // Push debug.traceback
                lua_getglobal(lua.lua_state(), "debug");
                lua_getfield(lua.lua_state(), -1, "traceback");
                lua_remove(lua.lua_state(), -2); // remove debug table
                int traceback_index = lua_gettop(lua.lua_state());

                // Call action
                sol::protected_function_result result = action();

                bool success = result.valid();
                if (!success)
                {
                    sol::error err = result;
                    ALK_ERROR("LuaRuntime: %s", err.what());
                }

                lua_remove(lua.lua_state(), traceback_index); // remove traceback
                return success;
            }
        };

        struct LuaNamespace
        {
            sol::state& lua;
            sol::table table;
            std::string name;

            LuaNamespace(sol::state& lua, const std::string& name) : lua(lua), name(name)
            {
                table = lua[name].get_or_create<sol::table>();
            }

            template <typename... Args>
            LuaNamespace& AddFunction(const std::string& fnName, Args&&... args)
            {
                table.set_function(fnName, std::forward<Args>(args)...);
                return *this;
            }
        };

        template <typename T>
        struct LuaUsertype
        {
            sol::state& lua;
            sol::usertype<T> type;
            std::string name;

            LuaUsertype(sol::state& lua, const std::string& name) : lua(lua), name(name)
            {
                type = lua.new_usertype<T>(name, sol::no_constructor);
            }

            template <typename... Args>
            LuaUsertype& SetConstructors(Args&&... args)
            {
                lua.set_function(name, sol::overload(std::forward<Args>(args)...));
                return *this;
            }

            template <typename... Args>
            LuaUsertype& AddFunction(const std::string& fnName, Args&&... args)
            {
                type.set_function(fnName, std::forward<Args>(args)...);
                return *this;
            }

            template <typename M>
            LuaUsertype& AddMember(const std::string& mName, M&& member)
            {
                type.set(mName, member);
                return *this;
            }

            template <typename F>
            LuaUsertype& SetToString(F&& fn)
            {
                type.set("__tostring", fn);
                return *this;
            }

        };

        inline sol::state& GetState()
        {
            static sol::state lua;
            return lua;
        }

        inline LuaSafeRunner& GetSafeRunner()
        {
            static LuaSafeRunner safe(GetState());
            return safe;
        }

        inline std::map<std::string, std::vector<sol::function>>& GetNotificationMap()
        {
            static std::map<std::string, std::vector<sol::function>> notificationMap;
            return notificationMap;
        }

        inline void RegisterNotification(const std::string& notification)
        {
            auto& notificationMap = GetNotificationMap();
            ALK_ASSERT(!notificationMap.contains(notification), "[ScriptSystem] Notification already registered: %s", notification.c_str());
            notificationMap.emplace(notification, std::vector<sol::function>());
        }

        inline void RegisterNotificationCallback(std::string notification, sol::function callback)
        {
            auto& notificationMap = GetNotificationMap();
            if (notificationMap.contains(notification))
            {
                notificationMap[notification].push_back(callback);
            }
        }

        template <typename... Args>
        inline void SendNotification(const std::string& notification, Args&&... args)
        {
            auto& notificationMap = GetNotificationMap();
            ALK_ASSERT(notificationMap.contains(notification), "[ScriptSystem] Attempting to send nonexisting notification: %s", notification.c_str());
            LuaSafeRunner& safe = GetSafeRunner();
            //IDEA: buffer notifications and send in Update
            for (auto& callback : notificationMap[notification])
            {
                safe.CallFunction(callback, std::forward<Args>(args)...);
            }
        }

        bool Initialize();
        void AddToPackage(const std::string& path);
        sol::table LoadTableFromFile(const std::string& filePath);
        void SaveTableToFile(const std::string& filePath, const sol::table& table);
        std::string TableToString(const sol::table& table, int indent = 1);

        inline bool RunFile(const std::string& filename)
        {
            return GetSafeRunner().RunFileSafe(filename);
        }

        template <typename... Args>
        bool CallFunction(const std::string& funcName, Args&&... args)
        {
            return GetSafeRunner().CallFunction(funcName, std::forward<Args>(args)...);
        }

        inline LuaNamespace CreateNamespace(const std::string& name)
        {
            return LuaNamespace(GetState(), name);
        }

        template<typename T>
        LuaUsertype<T> CreateUsertype(const std::string& name)
        {
            return LuaUsertype<T>(GetState(), name);
        }
    }
}