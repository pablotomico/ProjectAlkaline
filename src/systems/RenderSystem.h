#pragma once

#include "alkaline_lib.h"
#include <unordered_map>
#include <variant>
#include "include/raylib.h"

namespace alk
{
    class RenderComponent;
    class TransformComponent;

    namespace RenderSystem
    {
        Texture2D LoadRenderSystemTexture(const char* filename);

        struct TextureDeleter
        {
            void operator()(Texture2D* texture) const {
                if (texture) {
                    UnloadTexture(*texture);
                    delete texture;
                }
            }
        };

        enum struct RenderType
        {
            Sprite,
            SpriteArray,
        };
    
        struct SpriteRenderData
        {
            std::unique_ptr<Texture2D, TextureDeleter> texture;
            uint32_t drawLayer = 0;
            SpriteRenderData(){};
            SpriteRenderData(const char* spriteFilename){
                texture = std::unique_ptr<Texture2D, TextureDeleter>(
                    new Texture2D(LoadRenderSystemTexture(spriteFilename)),
                    TextureDeleter{}
                );
                ALK_LOG("Created Sprite Render Data");
            };
        };

        // Variant data types simplifies the use of different types of render data through the same system
        using VariantRenderData = std::variant<SpriteRenderData>;

        struct RenderSystemData
        {
            RenderSystemData()
            {
                ALK_LOG("Created Render System Data");
            };
            std::vector<std::pair<std::weak_ptr<RenderComponent>, std::weak_ptr<TransformComponent>>> drawables;
            bool dirtyLayers = false;
            std::unordered_map<const char*, Texture2D> loadedTextures;
        };

        inline RenderSystemData& GetRenderSystemData()
        {
            static RenderSystemData drawableData;
            return drawableData;
        }

        void Initialize();
        void Shutdown();
        void Draw();
        void DrawSprite(std::weak_ptr<RenderComponent> renderComponent, std::weak_ptr<TransformComponent> transformComponent);
        void DrawSpriteArray(std::weak_ptr<RenderComponent> renderComponent, std::weak_ptr<TransformComponent> transformComponent);

        // TODO: Figure out nicer way to add to screen
        void AddToScreen(std::pair<std::weak_ptr<RenderComponent>, std::weak_ptr<TransformComponent>> componentPair);
    }
}