#pragma once

#include "alkaline_lib.h"
#include "systems/World.h"
#include "systems/GridSystem.h"
#include "misc/GameState.h"

namespace alk
{
    namespace GameLogic
    {
        class Scene
        {
        private:
            // TODO: Investigate better way to render grid without requiring an entity
            Entity gridRenderEntity;
            // Entity entity;
            // Entity army;
            const int armySize = 50000;
            
            World world;
            GridSystem* gridSystem;
            
            Entity gridPlacementEntity;

            std::vector<Entity> buildings;

            GameState gameState;

            void OnGameStateChanged(EGameState oldState, EGameState newState);
        public:
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