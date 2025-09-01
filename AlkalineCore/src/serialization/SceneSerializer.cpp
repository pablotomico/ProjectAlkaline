#include "SceneSerializer.h"

#include "systems/Script/ScriptSystem.h"
#include "systems/GameLogic/Scene.h"
#include "entities/Entity.h"

void alk::SceneSerializer::DeserializeScene(alk::Scene &scene, const sol::table &table)
{
    alk::World& world = scene.GetWorld();
    scene.name = table["name"];
    ALK_LOG("SceneSerializer::DeserializeScene: '%s'", scene.name.c_str());

    sol::table entities = table["entities"];

    for (auto &pair : entities)
    {
        sol::table entity = pair.second;
        alk::SceneSerializer::DeserializeEntity(scene, entity);
    }
}

void alk::SceneSerializer::SerializeScene(alk::Scene &scene, sol::table& table)
{
    alk::World& world = scene.GetWorld();
    table["name"] = scene.name;
    sol::state* luaState = alk::ScriptSystem::GetState();
    sol::table entitiesTable = luaState->create_table();
    for (auto& pair : world.GetAllEntities())
    {
        EntityId id = pair.first;
        alk::EntityMeta& em = pair.second;
        if (em.valid)
        {
            sol::table entityTable = luaState->create_table();
            entityTable["name"] = em.name;
            sol::table componentsTable = luaState->create_table();

            for (auto& serializer : GetComponentSerializerList())
            {
                serializer(*luaState, id, world, componentsTable);
            }

            entityTable["components"] = componentsTable;
            entitiesTable.add(entityTable);
        }
    }
    table["entities"] = entitiesTable;
}

alk::Entity alk::SceneSerializer::DeserializeEntity(alk::Scene& scene, sol::table& entity)
{
    alk::World& world = scene.GetWorld();
    std::string name = entity["name"];
    sol::table components = entity["components"];

    Entity e = world.CreateEntity(name);
    for(auto& pair : components)
    {
        std::string componentName = pair.first.as<std::string>();
        sol::table componentTable = pair.second;

        GetComponentDeserializerMap()[componentName](e.id, world, componentTable);
    }

    return e;
}
