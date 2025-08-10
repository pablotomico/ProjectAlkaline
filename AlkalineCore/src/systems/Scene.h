#pragma once

#include <string>

#include "alkaline_lib.h"

#include "systems/World.h"
#include "systems/GridSystem.h"

namespace alk
{
    namespace GameLogic
    {
        enum EGameState
        {
            NONE,
            BUILD,
            BATTLE,
            REWARDS,
        };
        

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
            GridSystem* gridSystem;
            
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
}