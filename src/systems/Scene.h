#pragma once

#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "components/TransformComponent.h"

#include "types/Grid.h"

namespace alk
{
    namespace GameLogic
    {
        class Scene : public GameLogicSystem
        {
        private:
            Grid grid = Grid(3, 3, 128, Vector2{400.0f, 200.0f});

        public:
            Scene() : GameLogicSystem("Scene Subsystem") {}

            void Initialize() override
            {
                ALK_LOG("Initializing Scene subsystem");

                alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent *t)
                                                                         { OnTransformAdded(t); });
            }

            void Update() override
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    auto mousePosition = GetMousePosition();
                    auto gridPosition = grid.ScreenToGridPosition(mousePosition);

                    if (gridPosition.x > -1)
                    {
                        ALK_LOG("Clicked in pos (%g, %g)", gridPosition.x, gridPosition.y);
                    }
                    else
                    {
                        ALK_LOG("Clicked outside of the grid");
                    }
                }
            }

            void Draw() override
            {
                grid.Draw();
            }

            void Shutdown() override
            {
            }

        private:
            void OnTransformAdded(TransformComponent *t)
            {
                ALK_LOG("Trasnform added to scene");
            }
        };
    }
}