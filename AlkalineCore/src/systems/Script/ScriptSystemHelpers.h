#pragma once

#include "alkaline_lib.h"
#include "sol/sol.hpp"

namespace alk
{
    struct LuaSafeRunner
    {
        LuaSafeRunner(sol::state& lua) : lua(lua) {}

        // Run a Lua file with traceback
        sol::object RunFileSafe(const std::string& filename)
        {
            return RunWithTraceback([&]() { return lua.safe_script_file(filename, sol::script_pass_on_error); });
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
                }).valid();
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
                }).valid();
        }

    private:
        sol::state& lua;

        template <typename Callable>
        sol::object RunWithTraceback(Callable&& action)
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
                ALK_ASSERT(success, "LuaRuntime: %s", err.what())
            }

            sol::object obj = result;

            lua_remove(lua.lua_state(), traceback_index); // remove traceback
            return obj;
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
}