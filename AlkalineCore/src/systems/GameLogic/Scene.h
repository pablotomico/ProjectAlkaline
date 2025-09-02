#pragma once

#include <string>

#include "alkaline_lib.h"

#include "systems/GameLogic/World.h"

namespace alk
{
    enum EGameState
    {
        NONE,
        BUILD,
        BATTLE,
        REWARDS,
    };

    class GridLogicSubsystem;

    class Scene
    {
    private:
        // TODO: Investigate better way to render grid without requiring an entity
        Entity gridRenderEntity;
        Entity gamemodeEntity;
        // Entity entity;
        // Entity army;
        const int armySize = 50000;

        World world;
        Entity gridPlacementEntity;

        std::vector<Entity> buildings;

        void OnGameStateChanged(EGameState oldState, EGameState newState, const char* stateString);
    public:
        std::string name;
        Scene() = default;

        void Initialize();
        World& GetWorld()
        {
            return world;
        }

        void Update();
    };
}