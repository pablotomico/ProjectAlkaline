#include <map>

#include "raylib/raylib.h"

#include "systems/GameLogic.h"
#include "systems/ScriptSystem.h"
#include "systems/subsystems/GameLogicSubsystem.h"
#include "components/TransformComponent.h"
#include "GameLogic.h"

std::map<std::type_index, alk::GameLogic::SubsystemFactoryFn>& alk::GameLogic::GetFactoryList()
{
    static std::map<std::type_index, alk::GameLogic::SubsystemFactoryFn> factories;
    return factories;
}

void alk::GameLogic::Initialize(Scene scene)
{
    for (auto& pair : GetFactoryList())
    {
        auto system = pair.second();
        AddSubsystem(pair.first, system);
    }

    alk::ScriptSystem::CreateUsertype<EntityId>("Entity");
    alk::ScriptSystem::CreateUsertype<Vector2>("Vector")
        .SetConstructors(
            []() { return Vector2(); },
            [](float x, float y) { return Vector2(x, y); })
        .SetToString([](const Vector2& v) { return std::format("({}, {})", v.x, v.y); })
        .AddMember("x", &Vector2::x)
        .AddMember("y", &Vector2::y);

    alk::ScriptSystem::CreateNamespace("Game")
        .AddFunction("SpawnSigil", SpawnSigil)
        .AddFunction("GetRandomEntity", GetRandomEntity)
        .AddFunction("GetEntityPosition", GetEntityPosition)
        .AddFunction("SetEntityPosition", SetEntityPosition);

    GameLogic::LoadScene(std::move(scene), true);

    for (GameLogicSubsystem* system : GetSubsystems())
    {
        system->SetEnabled(system->Initialize());
    }
}

void alk::GameLogic::Update(const float deltaTime)
{
    for (GameLogicSubsystem* system : GetSubsystems())
    {
        if (system && system->IsEnabled())
        {
            system->Update();
        }
    }

    Scene* activeScene = GetActiveScene();
    if (activeScene)
    {
        activeScene->Update();
    }
}

void alk::GameLogic::Shutdown() {}

// TODO: Add validation to these
Vector2 alk::GameLogic::GetEntityPosition(EntityId id)
{
    auto c = GetWorld().GetComponent<TransformComponent>(id);
    return c ? c->position : Vector2{ -1, -1 };
}

void alk::GameLogic::SetEntityPosition(EntityId id, Vector2 position)
{
    GetWorld().GetComponent<TransformComponent>(id)->position = position;
}

alk::EntityId alk::GameLogic::GetRandomEntity()
{
    auto transforms = GetWorld().GetComponents<TransformComponent>();
    return transforms->Size() > 0 ? transforms->entities[0] : 0;
}

alk::EntityId alk::GameLogic::SpawnSigil(const std::string& path)
{
    std::string fullPath = std::string(GetWorkingDirectory()) + "/" + path;
    sol::table table = alk::ScriptSystem::LoadTableFromFile(fullPath);
    Entity e = alk::SceneSerializer::DeserializeEntity(*GetActiveScene(), table);
    ALK_LOG("[GameLogic] Spawned '%s' from sigil", e.name.c_str());
    alk::GameLogic::NotifyCallbacks(e.id);
    return e.id;
}