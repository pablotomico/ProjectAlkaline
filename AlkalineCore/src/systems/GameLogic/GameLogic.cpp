#include <map>

#include "raylib/raylib.h"

#include "systems/GameLogic/GameLogic.h"
#include "systems/GameLogic/GameLogicSubsystem.h"
#include "systems/Script/ScriptSystem.h"
#include "components/TransformComponent.h"
#include "GameLogic.h"

alk::GameLogic::GameLogic()
{
    GetSubsystems() = &subsystems;
    GetSubsystemsMap() = &subsystemsMap;
    GetCallbacks() = &callbacks;
    GetScenes() = &scenes;
}

std::map<std::type_index, alk::SubsystemFactoryFn>& alk::GameLogic::GetFactoryList()
{
    static std::map<std::type_index, alk::SubsystemFactoryFn> factories;
    return factories;
}

void alk::GameLogic::Initialize(Scene& scene)
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
        .AddFunction("SpawnSigil", &GameLogic::SpawnSigil, this)
        .AddFunction("GetRandomEntity", &GameLogic::GetRandomEntity, this)
        .AddFunction("GetEntityPosition", &GameLogic::GetEntityPosition, this)
        .AddFunction("SetEntityPosition", &GameLogic::SetEntityPosition, this);

    LoadScene(std::move(scene), true);

    for (GameLogicSubsystem* system : subsystems)
    {
        system->SetEnabled(system->Initialize());
    }
}

void alk::GameLogic::Reflect(ScriptSystem& script)
{

}

void alk::GameLogic::Update(const float deltaTime)
{
    for (GameLogicSubsystem* system : subsystems)
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

std::vector<alk::GameLogicSubsystem*>*& alk::GameLogic::GetSubsystems()
{
    static std::vector<GameLogicSubsystem*>* subsystems;
    return subsystems;
}

std::unordered_map<std::type_index, alk::GameLogicSubsystem*>*& alk::GameLogic::GetSubsystemsMap()
{
    static std::unordered_map<std::type_index, GameLogicSubsystem*>* subsystemsMap;
    return subsystemsMap;
}

std::vector<alk::EntityCallback>*& alk::GameLogic::GetCallbacks()
{
    static std::vector<EntityCallback>* callbacks;
    return callbacks;
}

void alk::GameLogic::AddSubsystem(std::type_index type, alk::GameLogicSubsystem* system)
{
    subsystems.push_back(system);
    subsystemsMap.emplace(type, system);
}

void alk::GameLogic::SubscribeToEntitySpawned(alk::EntityCallback callback)
{
    GetCallbacks()->push_back(callback);
}

void alk::GameLogic::NotifyCallbacks(EntityId id)
{
    for (auto& callback : *GetCallbacks())
    {
        callback(id);
    }
}

std::unordered_map<std::string, alk::Scene>*& alk::GameLogic::GetScenes()
{
    static std::unordered_map<std::string, Scene>* scenes;
    return scenes;
}

alk::Scene*& alk::GameLogic::GetActiveScene()
{
    static Scene* activeScene;
    return activeScene;
}

void alk::GameLogic::LoadScene(Scene scene, bool setActive)
{
    auto resultPair = scenes.emplace(scene.name, std::move(scene));
    if (resultPair.second)
    {
        Scene* loadedScene = &(resultPair.first->second);
        if (setActive)
        {
            GetActiveScene() = loadedScene;
        }
        loadedScene->Initialize();
        ALK_LOG("Successfully loaded scene %s", scene.name.c_str());
    }
}

alk::World& alk::GameLogic::GetWorld()
{
    return GetActiveScene()->GetWorld();
}