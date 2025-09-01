#pragma once

#include <vector>
#include <string>
#include <map>

namespace alk
{
    struct InputAction
    {
        std::string name;
        int key;
    };

    struct InputContext
    {
        std::string name;
        std::map<int, InputAction> actionMap;
    };
}