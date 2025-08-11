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
    }
}