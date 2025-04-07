#include "RenderSystem.h"
#include "components/RenderComponent.h"
#include "components/TransformComponent.h"

namespace alk
{
    void RenderSystem::Initialize()
    {
    }

    void RenderSystem::Shutdown()
    {
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
    void RenderSystem::AddToScreen(std::pair<std::weak_ptr<RenderComponent>, std::weak_ptr<TransformComponent>> componentPair)
    {
        RenderSystemData& renderData = GetRenderSystemData();
        renderData.drawables.push_back(componentPair);
        renderData.dirtyLayers = true;
    }
}
