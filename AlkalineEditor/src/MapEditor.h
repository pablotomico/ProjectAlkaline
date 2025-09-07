#pragma once

#include "alkaline_lib.h"
#include "EditorUtils.h"

#include <vector>

class MapChunk
{
public:
    MapChunk(int width, int height) : width(width), height(height)
    {
        Init();
    }

    MapChunk(int size) : width(size), height(size)
    {
        Init();
    }

    int* operator[](size_t i)
    {
        return &(tiles[i * width]);
    }

    size_t Size() const
    {
        return tiles.size();
    }

    int width;
    int height;
private:
    void Init()
    {
        tiles.reserve(width + height);

        for (int i = 0; i < tiles.capacity(); i++) {
            tiles.push_back(i);
        }
    }

    std::vector<int> tiles;
};

class MapEditor
{
public:
    MapEditor(bool& show, RenderTexture2D& texture) : show(show), texture(texture), map(10)
    {
        ALK_LOG("[MapEditor] Initilizng camera texture %dx%d", texture.texture.width, texture.texture.height);
        camera.target = Vector2(0, 0);
        camera.offset = Vector2(0, 0);
        // camera.offset = Vector2(texture.texture.width * 0.5f, 0);
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        tileSizeHalf = tileSize / 2;

        ALK_LOG("[MapEditor] test map operator %d", map[1][2]);
    }

    void Update(const float deltaTime);
    void Draw();

private:
    void DrawGrid();
    void DrawMapChunk(const MapChunk& map);
    
    void DrawToolbar();

    bool& show;
    RenderTexture2D& texture;
    Camera2D camera;
    MapChunk map;

    int tileSize = 64;
    int tileSizeHalf;
};