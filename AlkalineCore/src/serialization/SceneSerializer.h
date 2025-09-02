#pragma once

#include <map>
#include "sol.hpp"

#include "systems/Script/ScriptSystem.h"

#include "serialization/Serialization.h"

namespace alk
{
    class Scene;
    class World;

    namespace SceneSerializer
    {
        // #########################################
        // COMPONENT SERIALIZATION / DESERIALIZATION
        // #########################################
            
        using ComponentSerializerFn = void (*)(sol::state&, EntityId, alk::World&, sol::table&);

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

        using ComponentDeserializerFn = void (*)(EntityId, alk::World&, const sol::table&);

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
        
        void DeserializeScene(alk::Scene& scene, const sol::table& table);
        void SerializeScene(alk::Scene& scene, sol::table& table);
        Entity DeserializeEntity(alk::Scene& scene, sol::table& table);
    }
}