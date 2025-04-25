#include "systems/Scene.h"

#include "systems/World.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "types/Grid.h"

namespace alk
{
    namespace GameLogic
    {
        void Scene::Initialize()
        {
            ALK_LOG("Initializing Scene subsystem");

            alk::RenderSystem::AddToScreen(grid);

            entity = world.CreateEntity();
            world.AddComponent<RenderComponent>(entity, RenderSystem::RenderType::Sprite, "assets/sprites/test_building.png");
            world.AddComponent<TransformComponent>(entity, Vector2{50, 50});
            // entity.AddComponent<RenderComponent>(RenderSystem::RenderType::Sprite, "assets/sprites/test_building.png");
            // entity.AddComponent<TransformComponent>(Vector2{50, 50});
            alk::RenderSystem::AddToScreen(entity);

            army = world.CreateEntity();
            world.AddComponent<RenderComponent>(army, RenderSystem::RenderType::Sprite, "assets/sprites/test_unit.png");
            world.AddComponent<TransformComponent>(army, armySize);

            std::vector<Vector2> &positionArray = world.GetComponent<TransformComponent>(army)->GetPositionArray();
            for (int i = 0; i < armySize; ++i)
            {
                int rx = std::rand() % 50;
                int ry = std::rand() % 50;
                Vector2 gridPosition = grid.GridToScreenPosition(Vector2{float(rx), float(ry)});

                float dx = grid.tileWidthHalf - (std::rand() % grid.tileWidth);
                float dy = grid.tileHeightHalf - (std::rand() % grid.tileHeight);

                positionArray.emplace_back(Vector2{gridPosition.x - dx, gridPosition.y - dy});
            }
            alk::RenderSystem::AddToScreen(army);
        }

        void Scene::Update()
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
    }
}