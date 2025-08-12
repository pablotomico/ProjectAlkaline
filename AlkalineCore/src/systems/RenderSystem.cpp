#include "RenderSystem.h"

#include "systems/Scene.h"
#include "systems/GameLogic.h"

#include "components/RenderComponent.h"
#include "components/TransformComponent.h"
#include "components/GridPreviewComponent.h"
#include "components/GridComponent.h"
#include "components/GridEntityComponent.h"

#include "misc/GridHelpers.h"

// #include "tracy/Tracy.hpp"

namespace alk
{
    void RenderSystem::Initialize()
    {
        Camera2D &mainCamera = RenderSystem::GetMainCamera();
        mainCamera = {0};
        mainCamera.target = Vector2{0.0f, 0.0f};
        mainCamera.offset = Vector2{0.0f, 0.0f};
        mainCamera.rotation = 0.0f;
        mainCamera.zoom = 1.0f;
    }

    void RenderSystem::Shutdown()
    {
    }

    void RenderSystem::AddToScreen(Entity &entity)
    {
        alk::GameLogic::Scene *activeScene = alk::GameLogic::GetActiveScene();
        World &world = activeScene->GetWorld();
        world.GetComponent<RenderComponent>(entity)->SetVisible(true);
        RenderSystemData &renderData = GetRenderSystemData();
        renderData.dirtyLayers = true;

        ALK_LOG("Added entity to screen");
    }

    void RenderSystem::Draw()
    {
        // ZoneScoped;
        BeginMode2D(RenderSystem::GetMainCamera());

        EvaluateAndSortDirtyLayers();

        RenderSystemData &renderData = GetRenderSystemData();
        World &world = alk::GameLogic::GetWorld();
        for(auto entityId : renderData.drawables)
        {
            DrawEntity(entityId, &world);
        }

        EndMode2D();
    }

    void RenderSystem::EvaluateAndSortDirtyLayers()
    {
        RenderSystemData &renderData = GetRenderSystemData();
        if (renderData.dirtyLayers)
        {
            World &world = alk::GameLogic::GetWorld();
            auto renderComponents = world.GetComponents<RenderComponent>();
            auto gridEntityComponents = world.GetComponents<GridEntityComponent>();
            renderData.drawables.clear();

            for (auto i = 0; i < renderComponents->components.size(); ++i)
            {
                RenderComponent &renderComponent = renderComponents->components[i];
                EntityId id = renderComponents->entities[i];

                auto it = gridEntityComponents->entityIndices.find(id);
                if (it != gridEntityComponents->entityIndices.end())
                {
                    size_t gridEntityIndex = it->second;
                    GridEntityComponent &gridEntityComponent = gridEntityComponents->components[gridEntityIndex];
                    renderComponent.SetDrawLayer((uint)gridEntityComponent.GetGridPosition().x + (uint)gridEntityComponent.GetGridPosition().y);
                }

                renderData.drawables.push_back(id);
            }

            std::sort(renderData.drawables.begin(), renderData.drawables.end(), [&](EntityId a, EntityId b) {
                size_t aIndex = renderComponents->entityIndices[a];
                size_t bIndex = renderComponents->entityIndices[b];
                RenderComponent &aRenderComponent = renderComponents->components[aIndex];
                RenderComponent &bRenderComponent = renderComponents->components[bIndex];
                return aRenderComponent.GetDrawLayer() < bRenderComponent.GetDrawLayer();
            });

            ALK_LOG("Sorted render components by draw layer");
            renderData.dirtyLayers = false;
        }
    }

    void RenderSystem::DrawEntity(EntityId entityId, World *world)
    {
        Entity entity = world->GetEntity(entityId);
        if(!entity.IsValid())
        {
            return;
        }
        if(!world->HasComponent<RenderComponent>(entityId))
        {
            return;
        }
        
        RenderComponent *renderComponent = world->GetComponent<RenderComponent>(entity);
        TransformComponent *transformComponent = world->GetComponent<TransformComponent>(entity);
        if(renderComponent == nullptr || transformComponent == nullptr)
        {
            return;
        }
        if(!renderComponent->GetVisible())
        {
            return;
        }
        
        switch (renderComponent->GetRenderType())
        {
        case RenderSystem::RenderType::Sprite:
            DrawSprite(renderComponent, transformComponent);
            break;
        case RenderSystem::RenderType::Grid:
            alk::GameLogic::GridComponent *gridComponent = world->GetComponent<alk::GameLogic::GridComponent>(entity);
            DrawGrid(renderComponent, gridComponent);
            break;
        }
    }

    void RenderSystem::DrawSprite(RenderComponent *renderComponent, TransformComponent *transformComponent)
    {
        // ZoneScoped;
        RenderSystemData &renderSystemData = GetRenderSystemData();
        auto renderData = renderComponent->GetRenderData<SpriteRenderData>();
        Color color = renderComponent->GetColor();
        if (renderData)
        {
            const SpriteRenderData &data = renderData->get();
            auto tex = renderSystemData.loadedTextures[data.texHandler];
            DrawTexture(tex, (int) transformComponent->position.x, (int) transformComponent->position.y, color);
        }
        else
        {
            // TODO: Throw error for missing sprite render data
        }
    }

    void RenderSystem::DrawGrid(RenderComponent* renderComponent, alk::GameLogic::GridComponent* gridComponent)
    {
        // ZoneScoped;
        std::vector<Vector2> &gridPoints = gridComponent->gridPoints;
        auto renderData = renderComponent->GetRenderData<GridRenderData>()->get();

        for (Vector2 point : gridPoints)
        {
            auto endPosX = (int) point.x + renderData.tileWidthHalf;
            auto endPosY = (int) point.y + renderData.tileHeightHalf;
            DrawLine((int) point.x, (int) point.y, endPosX, endPosY, WHITE);
            DrawLine((int) point.x - renderData.tileWidthHalf, (int) point.y + renderData.tileHeightHalf, endPosX - renderData.tileWidthHalf, endPosY + renderData.tileHeightHalf, WHITE);

            DrawLine((int) point.x, (int) point.y, (int) point.x - renderData.tileWidthHalf, (int) point.y + renderData.tileHeightHalf, WHITE);
            DrawLine(endPosX, endPosY, endPosX - renderData.tileWidthHalf, endPosY + renderData.tileHeightHalf, WHITE);
        }

        World &world = alk::GameLogic::GetWorld();
        auto gridPreviewComponents = world.GetComponents<GridPreviewComponent>();

        if (gridPreviewComponents->components.size() == 0)
        {
            return;
        }

        RenderSystemData &renderSystemData = GetRenderSystemData();

        for (auto i = 0; i < gridPreviewComponents->components.size(); ++i)
        {
            auto &gridPreviewComponent = gridPreviewComponents->components[i];
            std::vector<std::pair<Vector2, bool>>& validMap = gridPreviewComponent.GetValidMap();

            for (size_t i = 0; i < validMap.size(); i++)
            {
                Vector2 gridPosition = validMap[i].first;
                Vector2 worldPosition = alk::GameLogic::GridHelpers::GridToWorldPosition(gridPosition, gridComponent->width, gridComponent->height);
                worldPosition.x -= (renderData.tileWidthHalf);
                if(validMap[i].second)
                {
                    auto tex = renderSystemData.loadedTextures[renderData.validTileTexHandler];
                    DrawTexture(tex, (int)worldPosition.x, (int)worldPosition.y, Color{ 255, 255, 255, 100 });
                }
                else
                {
                    auto tex = renderSystemData.loadedTextures[renderData.invalidTileTexHandler];
                    DrawTexture(tex, (int)worldPosition.x, (int)worldPosition.y, Color{ 255, 255, 255, 100 });
                }
            }
        }
    }

    TextureHandler RenderSystem::LoadRenderSystemTexture(const char *filename)
    {
        RenderSystemData &renderData = GetRenderSystemData();

        auto it = renderData.loadedHandlers.find(filename);
        if (it != renderData.loadedHandlers.end())
        {
            // Texture has already been previously loaded, return saved texture
            return it->second;
        }

        TextureHandler texHandler = GetNextTextureHandler();
        Texture2D texture = LoadTexture(filename);
        renderData.loadedHandlers.insert({filename, texHandler});
        renderData.loadedTextures.insert({texHandler, texture});
        return texHandler;
    }
}
