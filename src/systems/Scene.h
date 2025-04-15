#pragma once

#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "types/Grid.h"

namespace alk
{
    namespace GameLogic
    {
        class Scene : public GameLogicSystem
        {
        private:
            Grid* grid;
            BaseEntity* entity;
            BaseEntity* army;
            const int armySize = 50000;
        public:
            Scene() : GameLogicSystem("Scene Subsystem") {}

            void Initialize() override
            {
                ALK_LOG("Initializing Scene subsystem");

                grid = new Grid(50, 50, 32, Vector2{800.0f, 50.0f});
                alk::RenderSystem::AddToScreen(grid);

                entity = new BaseEntity();
                entity->AddComponent<RenderComponent>(RenderSystem::RenderType::Sprite, "assets/sprites/test_building.png");
                entity->AddComponent<TransformComponent>(Vector2{50, 50});
                alk::RenderSystem::AddToScreen(entity);

                army = new BaseEntity();
                army->AddComponent<RenderComponent>(RenderSystem::RenderType::Sprite, "assets/sprites/test_unit.png");
                army->AddComponent<TransformComponent>(armySize);

                std::vector<Vector2>& positionArray = army->GetComponent<TransformComponent>()->GetPositionArray();
                for (int i = 0; i < armySize; ++i)
                {
                    int rx = std::rand() % 50;
                    int ry = std::rand() % 50;
                    Vector2 gridPosition = grid->GridToScreenPosition(Vector2{float(rx), float(ry)});

                    float dx = grid->tileWidthHalf - (std::rand() % grid->tileWidth);
                    float dy = grid->tileHeightHalf - (std::rand() % grid->tileHeight);

                    positionArray.emplace_back(Vector2{gridPosition.x - dx, gridPosition.y - dy});
                }
                alk::RenderSystem::AddToScreen(army);

                alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent *t)
                                                                         { OnTransformAdded(t); });
            }

            void Update() override
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    auto mousePosition = GetMousePosition();
                    auto gridPosition = grid->ScreenToGridPosition(mousePosition);

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