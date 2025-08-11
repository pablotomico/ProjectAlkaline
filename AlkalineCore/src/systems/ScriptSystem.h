#pragma once

#include <string>

#include "sol.hpp"

#include "alkaline_lib.h"

namespace alk
{
    namespace ScriptSystem
    {
        inline sol::state& GetState()
        {
            static sol::state lua;
            return lua;
        }

        bool Initialize();
        sol::table LoadTableFromFile(const std::string& filePath);
        void SaveTableToFile(const std::string& filePath, const sol::table& table);

        std::string TableToString(const sol::table& table, int indent = 1);
    }
}