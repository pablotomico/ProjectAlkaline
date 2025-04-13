#include "RenderSystem.h"
#include "components/RenderComponent.h"
#include "components/TransformComponent.h"
#include "entities/BaseEntity.h"

namespace alk
{
    void RenderSystem::Initialize()
    {
    }

    void RenderSystem::Shutdown()
    {
    }

    void RenderSystem::AddToScreen(BaseEntity* entity)
    {
        RenderSystemData& renderData = GetRenderSystemData();
        auto componentPair = std::make_pair(entity->GetComponent<RenderComponent>(), entity->GetComponent<TransformComponent>());
        renderData.drawables.push_back(componentPair);
        renderData.dirtyLayers = true;
        ALK_LOG("Added entity to screen");
    }

    void RenderSystem::Draw()
    {
        RenderSystemData& renderData = GetRenderSystemData();
        if (renderData.dirtyLayers)
        {
            // TODO: Sort drawables by layer
            renderData.dirtyLayers = false;
        }

        for (auto it = renderData.drawables.begin(); it != renderData.drawables.end(); ++it) {
            std::weak_ptr<RenderComponent> renderComponent = it->first;
            std::weak_ptr<TransformComponent> transformComponent = it->second;

            switch(renderComponent.lock()->GetRenderType())
            {
                case RenderSystem::RenderType::Sprite:
                    DrawSprite(renderComponent, transformComponent);
                    break;
                case RenderSystem::RenderType::SpriteArray:
                    DrawSpriteArray(renderComponent, transformComponent);
                    break;
                case RenderSystem::RenderType::Grid:
                    DrawGrid(renderComponent, transformComponent);
                    break;
            }
        }
    }

    void RenderSystem::DrawSprite(std::weak_ptr<RenderComponent> renderComponent, std::weak_ptr<TransformComponent> transformComponent)
    {
        const Vector2 position = transformComponent.lock()->GetPosition();
        auto renderData = renderComponent.lock()->GetRenderData<SpriteRenderData>();
        if (renderData)
        {
            const SpriteRenderData& data = renderData->get();
            DrawTexture(*data.texture, position.x, position.y, WHITE);
        }
        else
        {
            // TODO Throw error for missing sprite render data
        }
    }

    void RenderSystem::DrawSpriteArray(std::weak_ptr<RenderComponent> renderComponent, std::weak_ptr<TransformComponent> transformComponent)
    {
    }

    void RenderSystem::DrawGrid(std::weak_ptr<RenderComponent> renderComponent, std::weak_ptr<TransformComponent> transformComponent)
    {
        std::vector<Vector2>& positionArray = transformComponent.lock()->GetPositionArray();
        auto renderData = renderComponent.lock()->GetRenderData<GridRenderData>()->get();

        for (Vector2 point : positionArray)
        {
            auto endPosX = point.x + renderData.tileWidthHalf;
            auto endPosY = point.y + renderData.tileHeightHalf;
            DrawLine(point.x, point.y, endPosX, endPosY, WHITE);
            DrawLine(point.x - renderData.tileWidthHalf, point.y + renderData.tileHeightHalf, endPosX - renderData.tileWidthHalf, endPosY + renderData.tileHeightHalf, WHITE);

            DrawLine(point.x, point.y, point.x - renderData.tileWidthHalf, point.y + renderData.tileHeightHalf, WHITE);
            DrawLine(endPosX, endPosY, endPosX - renderData.tileWidthHalf, endPosY + renderData.tileHeightHalf, WHITE);
        }
    }

    Texture2D RenderSystem::LoadRenderSystemTexture(const char* filename)
    {
        RenderSystemData& renderData = GetRenderSystemData();

        auto it = renderData.loadedTextures.find(filename);
        if (it != renderData.loadedTextures.end())
        {
            // Texture has already been previously loaded, return saved texture
            return it->second;
        }
        
        Texture2D texture = LoadTexture(filename);
        renderData.loadedTextures.insert({filename, texture});
        return texture;
    }
}
