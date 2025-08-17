#pragma once

#include <string>
#include "components/BaseComponent.h"
#include "serialization/Serialization.h"

namespace alk
{
    class ScriptComponent : public BaseComponent
    {
    public:
        std::string path;
        sol::table self;
        sol::function onStart;
        sol::function onUpdate;
        sol::function onStop;

        ALK_COMPONENT_SERIALIZER(ScriptComponent,
            {
                ALK_SERIALIZE_VALUE(path);
            });

        ALK_COMPONENT_DESERIALIZER(ScriptComponent,
            {
                ALK_DESERIALIZE_VALUE(path);
            });
    };
}