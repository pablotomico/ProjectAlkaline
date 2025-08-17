#pragma once

#include "alkaline_lib.h"
#include <unordered_map>
#include <variant>
#include "raylib.h"

#include "components/GridComponent.h"
#include "entities/Entity.h"

namespace alk
{
    using TextureHandler = uint32_t;

    class RenderComponent;
    class TransformComponent;
    class World;

    namespace RenderSystem
    {
        TextureHandler LoadRenderSystemTexture(const char* filename);

        struct TextureDeleter
        {
            void operator()(Texture2D* texture) const
            {
                if (texture)
                {
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

        struct RenderSystemData
        {
            RenderSystemData() {};
            std::vector<alk::EntityId> drawables;
            bool dirtyLayers = false;
            std::unordered_map<const char*, TextureHandler> loadedHandlers;
            std::unordered_map<TextureHandler, Texture2D> loadedTextures;
        };

        inline RenderSystemData& GetRenderSystemData()
        {
            static RenderSystemData drawableData;
            return drawableData;
        }

        struct SpriteRenderData
        {
            TextureHandler texHandler;
            uint16_t width;
            uint16_t height;

            SpriteRenderData() {};
            SpriteRenderData(const char* spriteFilename)
            {
                texHandler = LoadRenderSystemTexture(spriteFilename);
                width = GetRenderSystemData().loadedTextures[texHandler].width;
                height = GetRenderSystemData().loadedTextures[texHandler].height;
                ALK_LOG("Created Sprite Render Data");
            };
        };

        struct GridRenderData
        {
            TextureHandler validTileTexHandler;
            TextureHandler invalidTileTexHandler;
            uint tileWidthHalf;
            uint tileHeightHalf;
            GridRenderData() {};
            GridRenderData(uint tileWidthHalf, uint tileHeightHalf, const char* validTileTexFilename, const char* invalidTileTexFilename) : tileWidthHalf(tileWidthHalf), tileHeightHalf(tileHeightHalf)
            {
                validTileTexHandler = LoadRenderSystemTexture(validTileTexFilename);
                invalidTileTexHandler = LoadRenderSystemTexture(invalidTileTexFilename);
            };
        };

        // Variant data types simplifies the use of different types of render data through the same system
        using VariantRenderData = std::variant<SpriteRenderData, GridRenderData>;

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

        void AddToScreen(Entity& entity);
        void EvaluateAndSortDirtyLayers();
        void DrawEntity(EntityId entityId, World* world);
        void DrawSprite(RenderComponent* renderComponent, TransformComponent* transformComponent);
        void DrawGrid(RenderComponent* renderComponent, alk::GameLogic::GridComponent* gridComponent);
    }
}