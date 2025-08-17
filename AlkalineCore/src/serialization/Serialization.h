#pragma once

#include "sol/sol.hpp"
#include "systems/ScriptSystem.h"
#include "systems/World.h"
#include "entities/Entity.h"
#include "serialization/SceneSerializer.h"

#define ALK_SERIALIZE_VALUE(name) \
    table[#name] = component->##name;

#define ALK_DESERIALIZE_VALUE(name) \
    component->##name = table[#name];

#define ALK_SERIALIZE_VECTOR2(name)          \
    table[#name] = lua.create_table();       \
    table[#name]["x"] = component->##name.x; \
    table[#name]["y"] = component->##name.y;

#define ALK_DESERIALIZE_VECTOR2(name)        \
    component->##name.x = table[#name]["x"]; \
    component->##name.y = table[#name]["y"];

#define ALK_SERIALIZE(member)\
    typeid(member)

// static void SerializeProperty(typeId) // TODO here

#define ALK_COMPONENT_SERIALIZER(ComponentClass, BODY)                                  \
    static void Serialize(alk::EntityId entityId, alk::World &world, sol::table &componentsTable) \
    {                                                                                   \
        auto &lua = alk::ScriptSystem::GetState();                                      \
        ComponentClass *component = world.GetComponent<ComponentClass>(entityId);       \
        if (component == nullptr)                                                       \
            return;                                                                     \
        sol::table table = lua.create_table();                                          \
        BODY                                                                            \
        componentsTable[#ComponentClass] = table;                                       \
    }                                                                                   \
private:                                                                                \
    static inline bool isSerializerRegistered = alk::SceneSerializer::RegisterComponentSerializer<ComponentClass>(Serialize);

#define ALK_COMPONENT_DESERIALIZER(ComponentClass, BODY)                              \
    static void Deserialize(alk::EntityId entityId, alk::World &world, const sol::table &table) \
    {                                                                                 \
        world.AddComponent<ComponentClass>(entityId);                                 \
        ComponentClass *component = world.GetComponent<ComponentClass>(entityId);     \
        if (component == nullptr)                                                     \
            return;                                                                   \
        BODY                                                                          \
    }                                                                                 \
private:                                                                              \
    static inline bool isDeserializerRegistered = alk::SceneSerializer::RegisterComponentDeserializer<ComponentClass>(#ComponentClass, Deserialize);