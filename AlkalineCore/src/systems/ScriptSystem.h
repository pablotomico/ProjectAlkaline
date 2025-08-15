#pragma once

#include <string>

#include "sol.hpp"

#include "alkaline_lib.h"

namespace alk
{
    namespace ScriptSystem
    {
        class LuaSafeRunner {
        public:
            LuaSafeRunner(sol::state& lua) : lua(lua) {}

            // Run a Lua file with traceback
            bool RunFileSafe(const std::string& filename) {
                return RunWithTraceback([&]() {
                    return lua.script_file(filename, sol::script_pass_on_error);
                });
            }

            // Call a Lua function by name with traceback
            template <typename... Args>
            bool CallFunction(const std::string& funcName, Args&&... args) {
                return RunWithTraceback([&]() {
                    sol::protected_function func = lua[funcName];
                    if (!func.valid()) {
                        ALK_ERROR("LuaRuntime: '%s' is not a valid function", funcName);
                        return sol::protected_function_result();
                    }
                    return func(std::forward<Args>(args)...);
                });
            }

        private:
            sol::state& lua;

            template <typename Callable>
            bool RunWithTraceback(Callable&& action) {
                // Push debug.traceback
                lua_getglobal(lua.lua_state(), "debug");
                lua_getfield(lua.lua_state(), -1, "traceback");
                lua_remove(lua.lua_state(), -2); // remove debug table
                int traceback_index = lua_gettop(lua.lua_state());

                // Call action
                sol::protected_function_result result = action();

                bool success = result.valid();
                if (!success) {
                    sol::error err = result;
                    ALK_ERROR("LuaRuntime: %s", err.what());
                }

                lua_remove(lua.lua_state(), traceback_index); // remove traceback
                return success;
            }
        };

        struct LuaNamespace {
            sol::state& lua;
            sol::table table;
            std::string name;
            
            LuaNamespace(sol::state& lua, const std::string& name) : lua(lua), name(name) {
                table = lua[name].get_or_create<sol::table>();
            }
            
            template <typename FuncName, typename... Args>
            void AddFunction(FuncName name, Args&&... args)
            {
                table.set_function(name, std::forward<Args>(args)...);
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
    }
}