#include "SceneSerializer.h"

#include "systems/ScriptSystem.h"

void alk::SceneSerializer::DeserializeScene(alk::GameLogic::Scene &scene, const sol::table &table)
{
    World& world = scene.GetWorld();
    scene.name = table["name"];
    ALK_LOG("SceneSerializer::DeserializeScene: '%s'", scene.name.c_str());

    sol::table entities = table["entities"];

    for (auto &pair : entities)
    {
        sol::table entity = pair.second;
        std::string name = entity["name"];
        sol::table components = entity["components"];

        Entity e = world.CreateEntity(name);
        for(auto& pair : components)
        {
            // TODO: add components
        }
    }
}

void alk::SceneSerializer::SerializeScene(alk::GameLogic::Scene &scene, sol::table& table)
{
    World& world = scene.GetWorld();
    table["name"] = scene.name;
    sol::state& lua = alk::ScriptSystem::GetState();
    sol::table entitiesTable = lua.create_table();
    for (auto& pair : world.GetAllEntities())
    {
        EntityMeta& em = pair.second;
        if (em.valid)
        {
            sol::table entityTable = lua.create_table();
            entityTable["name"] = em.name;
            entityTable["components"] = lua.create_table();
            entitiesTable.add(entityTable);
        }
    }
    table["entities"] = entitiesTable;
}
