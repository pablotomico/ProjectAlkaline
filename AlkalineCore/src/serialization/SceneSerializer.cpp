#include "SceneSerializer.h"

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

sol::table alk::SceneSerializer::SerializeScene(alk::GameLogic::Scene &scene)
{
    return sol::table();
}
