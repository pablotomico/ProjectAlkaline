#pragma once

#include "entities/Entity.h"
#include "raylib/raylib.h"
#include <vector>
#include <unordered_map>

namespace alk
{
    using TextureHandle = size_t;

    struct RenderSystemData
    {
        std::vector<alk::EntityId> drawables;
        bool dirtyLayers = false;
        std::unordered_map<std::string, size_t> textureHandles;
        std::vector<Texture2D> loadedTextures;
    };

    struct RenderEntry
    {
        EntityId entityId;
        Vector2 projection; // cached isometric projection
        float sortKey;
        TextureHandle texHandle;
        Color color;
    };
}