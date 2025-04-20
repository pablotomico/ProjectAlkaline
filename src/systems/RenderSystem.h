#pragma once

#include "alkaline_lib.h"
#include <unordered_map>
#include <variant>
#include "include/raylib.h"

namespace alk
{
    using TextureHandler = uint32_t;

    class RenderComponent;
    class TransformComponent;
    class Entity;

    namespace RenderSystem
    {
        TextureHandler LoadRenderSystemTexture(const char* filename);

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
            Grid,
        };
    
        struct SpriteRenderData
        {
            TextureHandler texHandler;
            uint32_t drawLayer = 500;
            SpriteRenderData(){};
            SpriteRenderData(const char* spriteFilename){
                texHandler = LoadRenderSystemTexture(spriteFilename);
                ALK_LOG("Created Sprite Render Data");
            };
        };

        struct GridRenderData
        {
            uint32_t drawLayer = 100;
            uint tileWidthHalf;
            uint tileHeightHalf;
            GridRenderData(){};
            GridRenderData(uint tileWidthHalf, uint tileHeightHalf) : tileWidthHalf(tileWidthHalf), tileHeightHalf(tileHeightHalf){};
        };

        // Variant data types simplifies the use of different types of render data through the same system
        using VariantRenderData = std::variant<SpriteRenderData, GridRenderData>;

        struct RenderSystemData
        {
            RenderSystemData()
            {
                ALK_LOG("Created Render System Data");
            };
            std::vector<std::pair<std::weak_ptr<RenderComponent>, std::weak_ptr<TransformComponent>>> drawables;
            bool dirtyLayers = false;
            std::unordered_map<const char*, TextureHandler> loadedHandlers;
            std::unordered_map<TextureHandler, Texture2D> loadedTextures;
        };

        inline RenderSystemData& GetRenderSystemData()
        {
            static RenderSystemData drawableData;
            return drawableData;
        }

        inline TextureHandler GetNextTextureHandler()
        {
            static TextureHandler textureHandler = 0;
            return ++textureHandler;
        }

        inline Camera2D& GetMainCamera()
        {
            static Camera2D mainCamera;
            return mainCamera;
        }

        void Initialize();
        void Shutdown();
        void Draw();
        void DrawSprite(RenderComponent& renderComponent, TransformComponent& transformComponent);
        void DrawGrid(RenderComponent& renderComponent, TransformComponent& transformComponent);

        void AddToScreen(Entity& entity);
    }
}