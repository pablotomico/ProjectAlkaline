#pragma once

#include "entities/Entity.h"
#include "raylib/raylib.h"
#include <vector>
#include <unordered_map>

namespace alk
{
    using TextureHandler = unsigned int;

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

    struct RenderEntry
    {
        EntityId entityId;
        Vector2 projection; // cached isometric projection
        float sortKey;
        Texture2D texture;
        Color color;
    };
}