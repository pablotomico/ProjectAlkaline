#pragma once

#include <map>
#include "sol.hpp"

#include "systems/Scene.h"
#include "systems/World.h"
#include "systems/ScriptSystem.h"





namespace alk
{
    namespace SceneSerializer
    {
        class BaseComponent;

        // #######################
        // COMPONENT SERIALIZATION
        // #######################

        #define ALK_COMPONENT_SERIALIZER(ComponentClass, BODY)                                  \
            static void Serialize(EntityId entityId, World& world, sol::table& componentsTable) \
            {                                                                                   \
                auto& lua = alk::ScriptSystem::GetState();                                      \
                ComponentClass* component = world.GetComponent<ComponentClass>(entityId);       \
                if (component == nullptr) return;                                               \
                sol::table table = lua.create_table();                                          \
                BODY                                                                            \
                componentsTable[#ComponentClass] = table;                                       \
            }                                                                                   \
        private:                                                                                \
            static inline bool isSerializerRegistered = alk::SceneSerializer::RegisterComponentSerializer<ComponentClass>(Serialize);

            
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

        // #########################
        // COMPONENT DESERIALIZATION
        // #########################

        #define ALK_COMPONENT_DESERIALIZER(ComponentClass, BODY)                                \
            static void Deserialize(EntityId entityId, World& world, const sol::table& table)   \
            {                                                                                   \
                world.AddComponent<ComponentClass>(entityId);                                   \
                ComponentClass* component = world.GetComponent<ComponentClass>(entityId);       \
                if (component == nullptr) return;                                               \
                BODY                                                                            \
            }                                                                                   \
        private:                                                                                \
            static inline bool isDeserializerRegistered = alk::SceneSerializer::RegisterComponentDeserializer<ComponentClass>(#ComponentClass, Deserialize);

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