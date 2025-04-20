#pragma once

#include "alkaline_lib.h"

#include "systems/World.h"
#include "types/Grid.h"

namespace alk
{
    namespace GameLogic
    {
        class Scene
        {
        private:
            Grid grid{50, 50, 32, Vector2{800.0f, 50.0f}};
            Entity entity;
            Entity army;
            const int armySize = 50000;
            
            World world;

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