#include "GridSystem.h"
#include "GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/GridEntityComponent.h"

namespace alk
{
    namespace GameLogic
    {
        void GridSystem::Initialize()
        {
            ALK_LOG("Initializing Grid subsystem");
            positionArray.reserve(GRID_WIDTH * GRID_HEIGHT);

            for (int i = 0; i < GRID_WIDTH; ++i)
            {
                for (int j = 0; j < GRID_HEIGHT; ++j)
                {
                    Vector2 gridPosition = { float(i), float(j) };
                    Vector2 worldPosition = GridHelpers::GridToWorldPosition(gridPosition);
                    positionArray.emplace_back(worldPosition);
                }
            }
        }

        void GridSystem::Update()
        {
            alk::GameLogic::Scene *activeScene = alk::GameLogic::GetActiveScene();

            World &world = activeScene->GetWorld();
            auto renderComponents = world.GetComponents<RenderComponent>();
            auto transformComponents = world.GetComponents<TransformComponent>();
            auto gridPreviewComponents = world.GetComponents<GridPreviewComponent>();
            auto gridEntityComponents = world.GetComponents<GridEntityComponent>();

            for (auto i = 0; i < gridPreviewComponents->components.size(); ++i)
            {
                GridPreviewComponent &gridPreviewComponent = gridPreviewComponents->components[i];
                EntityId id = gridPreviewComponents->entities[i];

                size_t tranformIndex = transformComponents->entityIndices[id];
                TransformComponent &transformComponent = transformComponents->components[tranformIndex];

                size_t renderIndex = renderComponents->entityIndices[id];
                RenderComponent &renderComponent = renderComponents->components[renderIndex];

                Vector2 worldPosition = GetScreenToWorld2D(GetMousePosition(), alk::RenderSystem::GetMainCamera());

                auto gridPosition = GridHelpers::WorldToGridPosition(worldPosition);
                auto worldPlacementPosition = GridHelpers::GridToWorldPosition(gridPosition);

                // TODO Improve this so we don't directly reference render system
                // Set the preview sprite position to the center of the selected tile position
                const alk::RenderSystem::SpriteRenderData &renderData = renderComponent.GetRenderData<RenderSystem::SpriteRenderData>()->get();
                worldPlacementPosition.x -= (renderData.width/2);
                worldPlacementPosition.y = worldPlacementPosition.y - renderData.height + (TILE_HEIGHT * 2);

                transformComponent.SetPosition(worldPlacementPosition);
                gridPreviewComponent.SetGridPosition(gridPosition);
                
                EvaluateGridPreviewPlacement(gridPreviewComponent);
            }

            //TODO - Update to use notification rather than checking every frame
            for (auto i = 0; i < gridEntityComponents->components.size(); ++i)
            {
                GridEntityComponent &gridEntityComponent = gridEntityComponents->components[i];
                std::vector<Vector2> tileMap = gridEntityComponent.GetTileMap();
                for (size_t i = 0; i < tileMap.size(); i++)
                {
                    Vector2 gridPosition = tileMap[i];
                    if (gridPosition.x < 0 || gridPosition.x >= GRID_WIDTH || gridPosition.y < 0 || gridPosition.y >= GRID_HEIGHT)
                    {
                        continue;
                    }
                    gridState[(int)gridPosition.x][(int)gridPosition.y] = GridHelpers::GRID_POINT_FILLED;
                }
                
            }
            
        }

        void GridSystem::Shutdown()
        {
            ALK_LOG("Shutting down Grid subsystem");
        }

        void GridSystem::EvaluateGridPreviewPlacement(GridPreviewComponent &gridPreviewComponent)
        {
            std::vector<std::pair<Vector2, bool>>& validMap = gridPreviewComponent.GetValidMap();
            Vector2 gridPosition = gridPreviewComponent.GetGridPosition();
            Vector2 centerOffset = CalculateGridPreviewEntityCenterOffset(gridPreviewComponent);

            int tileIndex = 0;
            for (int i = 0; i < gridPreviewComponent.GetDimensions().x; ++i)
            {
                for (int j = 0; j < gridPreviewComponent.GetDimensions().y; ++j)
                {
                    Vector2 gridPositionOffset = { static_cast<float>(i + gridPosition.x - centerOffset.x),  static_cast<float>(j + gridPosition.y - centerOffset.y) };
                    if (EvaluateTileValid(gridPositionOffset))
                    {
                        validMap[tileIndex].first = {gridPositionOffset.x, gridPositionOffset.y};
                        validMap[tileIndex].second = true;
                    }
                    else
                    {
                        validMap[tileIndex].first = {gridPositionOffset.x, gridPositionOffset.y};
                        validMap[tileIndex].second = false;
                    }
                    tileIndex++;
                }
            }
        }

        bool GridSystem::EvaluateTileValid(Vector2 gridPosition)
        {
            if(gridPosition.x < 0 || gridPosition.x >= GRID_WIDTH || gridPosition.y < 0 || gridPosition.y >= GRID_HEIGHT)
            {
                return false;
            }
            if (gridState[(int)gridPosition.x][(int)gridPosition.y] != GridHelpers::GRID_POINT_EMPTY)
            {
                return false;
            }
            return true;
        }

        Vector2 GridSystem::CalculateGridPreviewEntityCenterOffset(GridPreviewComponent &gridPreviewComponent)
        {
            Vector2 dimensions = gridPreviewComponent.GetDimensions();
            Vector2 centerOffset = { 0, 0 };
            centerOffset.x = floor(dimensions.x / 2);
            centerOffset.y = floor(dimensions.y / 2);

            return centerOffset;
        }
    }
}