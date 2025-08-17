#include "systems/GameLogic.h"
#include "systems/subsystems/GameLogicSubsystem.h"
#include "systems/subsystems/GridSubsystem.h"

#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/GridEntityComponent.h"
#include "components/GridPreviewComponent.h"

namespace alk
{
    namespace GameLogic
    {
        bool GridSubsystem::Initialize()
        {
            World& world = alk::GameLogic::GetActiveScene()->GetWorld();
            auto gridComponents = world.GetComponents<GridComponent>();

            if (gridComponents->Size() == 0 ) return false;

            auto tranformComponents = world.GetComponents<TransformComponent>();

            for (auto i = 0; i < gridComponents->Size(); ++i)
            {
                auto pair = gridComponents->Get(i);
                TransformComponent* transform = tranformComponents->Get(pair.first);
                pair.second->GeneratePoints(transform->position);
            }

            
            return true;
        }

        void GridSubsystem::Update()
        {
            World &world = alk::GameLogic::GetActiveScene()->GetWorld();

            auto renderComponents = world.GetComponents<RenderComponent>();
            auto transformComponents = world.GetComponents<TransformComponent>();
            auto gridComponents = world.GetComponents<GridComponent>();
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
                auto worldPlacementPosition = Vector2{0, 0};
                for (auto grid : gridComponents->components)
                {
                    worldPlacementPosition = GridHelpers::GridToWorldPosition(gridPosition, grid.width, grid.height);
                    if (worldPlacementPosition.x > -1) 
                        break;
                }

                // TODO Improve this so we don't directly reference render system
                // Set the preview sprite position to the center of the selected tile position
                const alk::RenderSystem::SpriteRenderData &renderData = renderComponent.GetRenderData<RenderSystem::SpriteRenderData>()->get();
                worldPlacementPosition.x -= (renderData.width/2);
                worldPlacementPosition.y = worldPlacementPosition.y - renderData.height + (TILE_HEIGHT * 2);

                transformComponent.position = worldPlacementPosition;
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
                    gridState[(int)gridPosition.x][(int)gridPosition.y] = GridHelpers::GridPointState::GRID_POINT_FILLED;
                }

                EntityId id = gridEntityComponents->entities[i];
                size_t tranformIndex = transformComponents->entityIndices[id];
                TransformComponent &transformComponent = transformComponents->components[tranformIndex];

                Vector2 convertedGridPosition = GridHelpers::WorldToGridPosition(transformComponent.position);
                gridEntityComponent.SetGridPosition(convertedGridPosition);
            }
        }

        void GridSubsystem::Shutdown()
        {
            ALK_LOG("Shutting down Grid subsystem");
        }

        void GridSubsystem::EvaluateGridPreviewPlacement(GridPreviewComponent &gridPreviewComponent)
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

        bool GridSubsystem::EvaluateTileValid(Vector2 gridPosition)
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

        Vector2 GridSubsystem::CalculateGridPreviewEntityCenterOffset(GridPreviewComponent &gridPreviewComponent)
        {
            Vector2 dimensions = gridPreviewComponent.GetDimensions();
            Vector2 centerOffset = { 0, 0 };
            centerOffset.x = floor(dimensions.x / 2);
            centerOffset.y = floor(dimensions.y / 2);

            return centerOffset;
        }
    }
}