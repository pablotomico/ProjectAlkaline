#pragma once

#include <vector>
#include <functional>
#include <typeindex>
#include <map>

#include "raylib/raylib.h"
#include "alkaline_lib.h"

#include "systems/Scene.h"
#include "entities/Entity.h"

namespace alk
{
    namespace GameLogic
    {
        void Initialize(Scene scene);
        void Update(const float deltaTime);
        void Shutdown();
        
        class GameLogicSubsystem;
        using SubsystemFactoryFn = GameLogicSubsystem* (*)();

        std::map<std::type_index, alk::GameLogic::SubsystemFactoryFn>& GetFactoryList();

        template <typename T>
        inline bool RegisterSystemFactory(alk::GameLogic::SubsystemFactoryFn factory)
        {
            GetFactoryList().emplace(typeid(T), factory);
            return true;
        }


        inline std::vector<GameLogicSubsystem*>& GetSubsystems()
        {
            static std::vector<GameLogicSubsystem*> subsystems;
            return subsystems;
        }

        inline std::unordered_map<std::type_index, GameLogicSubsystem*>& GetSubsystemsMap()
        {
            static std::unordered_map<std::type_index, GameLogicSubsystem*> subsystemsMap;
            return subsystemsMap;
        }

        template <typename T>
        inline T* GetSubsystem()
        {
            return static_cast<T*>(GetSubsystemsMap()[typeid(T)]);
        }

        inline std::vector<std::function<void(EntityId)>>& GetCallbacks()
        {
            static std::vector<std::function<void(EntityId)>> callbacks;
            return callbacks;
        }

        inline void AddSubsystem(std::type_index type, GameLogicSubsystem* system)
        {
            GetSubsystems().push_back(system);
            GetSubsystemsMap().emplace(type, system);
        }

        inline void SubscribeToEntitySpawned(std::function<void(EntityId)> callback)
        {
            GetCallbacks().push_back(callback);
        }

        inline void NotifyCallbacks(EntityId id)
        {
            for (auto& callback : GetCallbacks())
            {
                callback(id);
            }
        }

        // Scene
        inline std::unordered_map<std::string, Scene>& GetScenes()
        {
            static std::unordered_map<std::string, Scene> scenes;
            return scenes;
        }

        inline Scene*& GetActiveScene()
        {
            static Scene* activeScene;
            return activeScene;
        }

        inline void LoadScene(Scene scene, bool setActive)
        {
            auto resultPair = GetScenes().emplace(scene.name, std::move(scene));
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

        inline World& GetWorld()
        {
            return GetActiveScene()->GetWorld();
        }

        Vector2 GetEntityPosition(EntityId id);
        void SetEntityPosition(EntityId id, Vector2 position);
        EntityId GetRandomEntity();
        
        EntityId SpawnSigil(const std::string& name);
    }
}