#pragma once

#include <map>
#include "sol.hpp"

#include "systems/Scene.h"
#include "systems/World.h"
#include "systems/ScriptSystem.h"

#include "serialization/Serialization.h"

namespace alk
{
    namespace SceneSerializer
    {
        class BaseComponent;

        // #########################################
        // COMPONENT SERIALIZATION / DESERIALIZATION
        // #########################################
            
        using ComponentSerializerFn = void (*)(EntityId, World&, sol::table&);

        inline std::vector<ComponentSerializerFn>& GetComponentSerializerList()
        {
            static std::vector<ComponentSerializerFn> serializers;
            return serializers;
        }

        template <typename T>
        inline bool RegisterComponentSerializer(ComponentSerializerFn fn)
        {
            GetComponentSerializerList().push_back(fn);
            return true;
        }

        using ComponentDeserializerFn = void (*)(EntityId, World&, const sol::table&);

        inline std::map<std::string, ComponentDeserializerFn>& GetComponentDeserializerMap()
        {
            static std::map<std::string, ComponentDeserializerFn> deserializers;
            return deserializers;
        }

        template <typename T>
        inline bool RegisterComponentDeserializer(std::string name, ComponentDeserializerFn fn)
        {
            GetComponentDeserializerMap().emplace(name, fn);
            return true;
        }

        // ######################################
        // SCENES SERIALIZATION / DESERIALIZATION
        // ######################################
        
        void DeserializeScene(alk::GameLogic::Scene& scene, const sol::table& table);
        void SerializeScene(alk::GameLogic::Scene& scene, sol::table& table);
    }
}