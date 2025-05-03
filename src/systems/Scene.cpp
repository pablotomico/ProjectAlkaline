#include "systems/Scene.h"
#include "systems/World.h"
#include "GridSystem.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/GridPreviewComponent.h"

namespace alk
{
    namespace GameLogic
    {
        void Scene::Initialize()
        {
            ALK_LOG("Initializing Scene subsystem");

            gridSystem = new GridSystem();
            gridSystem->Initialize();

            // TODO: Figure out a better way to render the grid on screen
            gridRenderEntity = world.CreateEntity();
            world.AddComponent<RenderComponent>(gridRenderEntity, RenderSystem::RenderType::Grid, TILE_WIDTH_HALF, TILE_HEIGHT_HALF);
            world.AddComponent<TransformComponent>(gridRenderEntity, gridSystem->GetGridArray().size());
            world.GetComponent<TransformComponent>(gridRenderEntity)->GetPositionArray() = gridSystem->GetGridArray();
            alk::RenderSystem::AddToScreen(gridRenderEntity);

            // army = world.CreateEntity();
            // world.AddComponent<RenderComponent>(army, RenderSystem::RenderType::Sprite, "assets/sprites/test_unit.png");
            // world.AddComponent<TransformComponent>(army, armySize);

            // std::vector<Vector2> &positionArray = world.GetComponent<TransformComponent>(army)->GetPositionArray();
            // for (int i = 0; i < armySize; ++i)
            // {
            //     int rx = std::rand() % 50;
            //     int ry = std::rand() % 50;
            //     Vector2 gridPosition = grid.GridToScreenPosition(Vector2{float(rx), float(ry)});

            //     float dx = grid.GetTileWidthHalf() - (std::rand() % grid.GetTileWidth());
            //     float dy = grid.GetTileHeightHalf() - (std::rand() % grid.GetTileHeight());

            //     positionArray.emplace_back(Vector2{gridPosition.x - dx, gridPosition.y - dy});
            // }
            //alk::RenderSystem::AddToScreen(army);
        }

        void Scene::Update()
        {
            // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            // {
            //     Vector2 worldPosition = GetScreenToWorld2D(GetMousePosition(), alk::RenderSystem::GetMainCamera());
            //     auto gridPosition = GridHelpers::WorldToGridPosition(worldPosition);

            //     if (gridPosition.x > -1)
            //     {
            //         ALK_LOG("Clicked in pos (%g, %g)", gridPosition.x, gridPosition.y);
            //     }
            //     else
            //     {
            //         ALK_LOG("Clicked outside of the grid");
            //     }
            // }

            if (IsKeyPressed(KEY_SPACE)) {
                if(!world.IsValid(gridPlacementEntity))
                {
                    gridPlacementEntity = world.CreateEntity();
                    world.AddComponent<RenderComponent>(gridPlacementEntity, RenderSystem::RenderType::Sprite, "assets/sprites/test_castle.png", Color{ 255, 255, 255, 100 });
                    world.AddComponent<TransformComponent>(gridPlacementEntity, Vector2{0, 0});
                    world.AddComponent<GridPreviewComponent>(gridPlacementEntity, Vector2{3, 3});
                    alk::RenderSystem::AddToScreen(gridPlacementEntity);
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if(world.IsValid(gridPlacementEntity))
                {
                    Entity newBuilding = world.CreateEntity();
                    buildings.push_back(newBuilding);
                    world.AddComponent<RenderComponent>(newBuilding, RenderSystem::RenderType::Sprite, "assets/sprites/test_castle.png", Color{ 255, 255, 255, 255 });

                    Vector2 position = world.GetComponent<TransformComponent>(gridPlacementEntity)->GetPosition();

                    world.AddComponent<TransformComponent>(newBuilding, position);
                    alk::RenderSystem::AddToScreen(newBuilding);

                    world.DestroyEntity(gridPlacementEntity);
                }
            }
        }
    }
}