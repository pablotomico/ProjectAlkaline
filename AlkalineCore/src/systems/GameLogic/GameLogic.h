#pragma once

#include "systems/BaseSystem.h"

#include <vector>
#include <functional>
#include <typeindex>
#include <map>

#include "raylib/raylib.h"
#include "alkaline_lib.h"

#include "systems/GameLogic/Scene.h"
#include "entities/Entity.h"

namespace alk
{
    class GameLogicSubsystem;
    using SubsystemFactoryFn = GameLogicSubsystem * (*)(CoreSystems&);
    using EntityCallback = std::function<void(EntityId)>;

    class GameLogic : public BaseSystem
    {
        std::unordered_map<std::type_index, GameLogicSubsystem*> subsystemsMap;
        std::vector<GameLogicSubsystem*> subsystems;
        std::vector<EntityCallback> callbacks;
        std::unordered_map<std::string, Scene> scenes;
    public:
        GameLogic(CoreSystems& coreSystems);
        void Initialize(Scene& scene) override;
        void Reflect(ScriptSystem& script) override;
        void Update(const float deltaTime) override;
        void Shutdown() override;

        Vector2 GetEntityPosition(EntityId id);
        void SetEntityPosition(EntityId id, Vector2 position);
        EntityId GetRandomEntity();
        EntityId SpawnSigil(const std::string& name);

        static std::map<std::type_index, alk::SubsystemFactoryFn>& GetFactoryList();
        static std::vector<GameLogicSubsystem*>*& GetSubsystems();
        static std::unordered_map<std::type_index, GameLogicSubsystem*>*& GetSubsystemsMap();

        template <typename T>
        inline T* GetSubsystem()
        {
            return static_cast<T*>(GetSubsystemsMap()[typeid(T)]);
        }

        template <typename T>
        inline static bool RegisterSystemFactory(alk::SubsystemFactoryFn factory)
        {
            GetFactoryList().emplace(typeid(T), factory);
            return true;
        }

        void AddSubsystem(std::type_index type, GameLogicSubsystem* system);
        void NotifyCallbacks(EntityId id);
        static std::vector<EntityCallback>*& GetCallbacks();
        static void SubscribeToEntitySpawned(EntityCallback callback);

        // Scene
        void LoadScene(Scene scene, bool setActive);

        static std::unordered_map<std::string, Scene>*& GetScenes();
        static Scene*& GetActiveScene();
        static World& GetWorld();
    };
}