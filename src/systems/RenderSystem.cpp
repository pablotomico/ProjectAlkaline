#include "RenderSystem.h"
#include "components/RenderComponent.h"
#include "components/TransformComponent.h"
#include "components/GridPreviewComponent.h"
#include "entities/Entity.h"
#include "systems/GameLogic.h"
#include "systems/Scene.h"
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

        RenderSystemData &renderData = GetRenderSystemData();
        if (renderData.dirtyLayers)
        {
            // TODO: Sort drawables by layer
            renderData.dirtyLayers = false;
        }

        World &world = alk::GameLogic::GetWorld();
        auto renderComponents = world.GetComponents<RenderComponent>();
        auto transformComponents = world.GetComponents<TransformComponent>();

        for (auto i = 0; i < renderComponents->components.size(); ++i)
        {
            RenderComponent &renderComponent = renderComponents->components[i];

            EntityId id = renderComponents->entities[i];

            ALK_ASSERT(world.HasComponent<TransformComponent>(id), "{} doesn't have a TransformComponent!", world.GetEntity(id).name.c_str());
            size_t tranformIndex = transformComponents->entityIndices[id];
            TransformComponent &transformComponent = transformComponents->components[tranformIndex];

            switch (renderComponent.GetRenderType())
            {
            case RenderSystem::RenderType::Sprite:
                DrawSprite(renderComponent, transformComponent);
                break;
            case RenderSystem::RenderType::Grid:
                DrawGrid(renderComponent, transformComponent);
                break;
            }
        }

        EndMode2D();
    }

    void RenderSystem::DrawSprite(RenderComponent &renderComponent, TransformComponent &transformComponent)
    {
        // ZoneScoped;
        RenderSystemData &renderSystemData = GetRenderSystemData();
        std::vector<Vector2> &positionArray = transformComponent.GetPositionArray();
        auto renderData = renderComponent.GetRenderData<SpriteRenderData>();
        Color color = renderComponent.GetColor();
        if (renderData)
        {
            const SpriteRenderData &data = renderData->get();
            auto tex = renderSystemData.loadedTextures[data.texHandler];
            // ZoneValue(positionArray.size());
            for (Vector2 &position : positionArray)
            {
                DrawTexture(tex, position.x, position.y, color);
            }
        }
        else
        {
            // TODO Throw error for missing sprite render data
        }
    }

    void RenderSystem::DrawGrid(RenderComponent &renderComponent, TransformComponent &transformComponent)
    {
        // ZoneScoped;
        std::vector<Vector2> &positionArray = transformComponent.GetPositionArray();
        auto renderData = renderComponent.GetRenderData<GridRenderData>()->get();

        for (Vector2 point : positionArray)
        {
            auto endPosX = point.x + renderData.tileWidthHalf;
            auto endPosY = point.y + renderData.tileHeightHalf;
            DrawLine(point.x, point.y, endPosX, endPosY, WHITE);
            DrawLine(point.x - renderData.tileWidthHalf, point.y + renderData.tileHeightHalf, endPosX - renderData.tileWidthHalf, endPosY + renderData.tileHeightHalf, WHITE);

            DrawLine(point.x, point.y, point.x - renderData.tileWidthHalf, point.y + renderData.tileHeightHalf, WHITE);
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
                Vector2 worldPosition = alk::GameLogic::GridHelpers::GridToWorldPosition(gridPosition);
                worldPosition.x -= (renderData.tileWidthHalf);
                if(validMap[i].second)
                {
                    auto tex = renderSystemData.loadedTextures[renderData.validTileTexHandler];
                    DrawTexture(tex, worldPosition.x, worldPosition.y, Color{ 255, 255, 255, 100 });
                }
                else
                {
                    auto tex = renderSystemData.loadedTextures[renderData.invalidTileTexHandler];
                    DrawTexture(tex, worldPosition.x, worldPosition.y, Color{ 255, 255, 255, 100 });
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
