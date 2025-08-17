#include "SceneSerializer.h"

#include "systems/ScriptSystem.h"
#include "systems/Scene.h"

void alk::SceneSerializer::DeserializeScene(alk::GameLogic::Scene &scene, const sol::table &table)
{
    alk::GameLogic::World& world = scene.GetWorld();
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
            std::string componentName = pair.first.as<std::string>();
            sol::table componentTable = pair.second;

            GetComponentDeserializerMap()[componentName](e.id, world, componentTable);
        }
    }
}

void alk::SceneSerializer::SerializeScene(alk::GameLogic::Scene &scene, sol::table& table)
{
    alk::GameLogic::World& world = scene.GetWorld();
    table["name"] = scene.name;
    sol::state& lua = alk::ScriptSystem::GetState();
    sol::table entitiesTable = lua.create_table();
    for (auto& pair : world.GetAllEntities())
    {
        EntityId id = pair.first;
        alk::GameLogic::EntityMeta& em = pair.second;
        if (em.valid)
        {
            sol::table entityTable = lua.create_table();
            entityTable["name"] = em.name;
            sol::table componentsTable = lua.create_table();

            for (auto& serializer : GetComponentSerializerList())
            {
                serializer(id, world, componentsTable);
            }

            entityTable["components"] = componentsTable;
            entitiesTable.add(entityTable);
        }
    }
    table["entities"] = entitiesTable;
}
