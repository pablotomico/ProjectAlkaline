#pragma once

#include <functional>

#include "sol.hpp"

#include "entities/Entity.h"

namespace alk
{

    using DeserializeFunc = std::function<void>(EntityId, sol::table);
    using SerializeFunc = std::function<sol::table>(EntityId);

    struct ComponentSerializationFuncs
    {
        DeserializeFunc deserialize;
        SerializeFunc serialize;
    };

    class ComponentRegistry
    {
    public:
        static void Register(const std::string& name, ComponentSerializationFuncs funcs);
        static ComponentSerializationFuncs* Get(const std::string& name);
    };
}