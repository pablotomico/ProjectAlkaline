#include "GridSystem.h"
#include "GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/GridPreviewComponent.h"

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

            if (gridPreviewComponents->components.size() == 0)
            {
                return;
            }

            for (auto i = 0; i < gridPreviewComponents->components.size(); ++i)
            {
                EntityId id = gridPreviewComponents->entities[i];

                size_t tranformIndex = transformComponents->entityIndices[id];
                TransformComponent &transformComponent = transformComponents->components[tranformIndex];

                size_t renderIndex = renderComponents->entityIndices[id];
                RenderComponent &renderComponent = renderComponents->components[renderIndex];

                Vector2 worldPosition = GetScreenToWorld2D(GetMousePosition(), alk::RenderSystem::GetMainCamera());

                auto gridPosition = GridHelpers::WorldToGridPosition(worldPosition);
                auto worldPlacementPosition = GridHelpers::GridToWorldPosition(gridPosition);

                const alk::RenderSystem::SpriteRenderData &renderData = renderComponent.GetRenderData<RenderSystem::SpriteRenderData>()->get();
                worldPlacementPosition.x -= (renderData.width/2);
                worldPlacementPosition.y = worldPlacementPosition.y - renderData.height + (TILE_HEIGHT * 2);

                transformComponent.SetPosition(worldPlacementPosition);
            }
        }

        void GridSystem::Shutdown()
        {
            ALK_LOG("Shutting down Grid subsystem");
        }
    }
}